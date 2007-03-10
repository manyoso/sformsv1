/* This file is part of KDevelop
    Copyright (C) 2006 Hamish Rodda <rodda@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "typebuilder.h"

#include "cppeditorintegrator.h"
#include "name_compiler.h"
#include "ducontext.h"
#include "cpptypes.h"
#include "parsesession.h"
#include "tokens.h"
#include "declaration.h"
#include "typerepository.h"

TypeBuilder::TypeBuilder(ParseSession* session)
  : TypeBuilderBase(session)
{
}

TypeBuilder::TypeBuilder(CppEditorIntegrator * editor)
  : TypeBuilderBase(editor)
{
}

void TypeBuilder::supportBuild(AST *node)
{
  m_topTypes.clear();

  TypeBuilderBase::supportBuild(node);

  Q_ASSERT(m_typeStack.isEmpty());
}

void TypeBuilder::openAbstractType(AbstractType::Ptr type, AST* node)
{
  Q_UNUSED(node);

  m_typeStack.append(type);
}

void TypeBuilder::closeType()
{
  // Check that this isn't the same as a previously existing type
  // If it is, it will get replaced
  m_lastType = TypeRepository::self()->registerType(currentAbstractType());

  bool replaced = m_lastType != currentAbstractType();

  // And the reference will be lost...
  m_typeStack.pop();

  if (!hasCurrentType() && !replaced)
    m_topTypes.append(m_lastType);
}

static CppClassType::Ptr openClass(int kind)
{
  CppClassType::Ptr classType(new CppClassType());

  if (kind == Token_struct)
    classType->setClassType(CppClassType::Struct);
  else if (kind == Token_union)
    classType->setClassType(CppClassType::Union);

  return classType;
}

void TypeBuilder::visitClassSpecifier(ClassSpecifierAST *node)
{
  CppClassType::Ptr classType = openClass(m_editor->parseSession()->token_stream->kind(node->class_key));
  openType(classType, node);

  TypeBuilderBase::visitClassSpecifier(node);

  // Prevent additional elements being added if this becomes the current type again
  classType->close();

  closeType();
}

void TypeBuilder::visitBaseSpecifier(BaseSpecifierAST *node)
{
  if (node->name) {
    QualifiedIdentifier baseClassIdentifier = identifierForName(node->name);
  }

  TypeBuilderBase::visitBaseSpecifier(node);
}

void TypeBuilder::visitEnumSpecifier(EnumSpecifierAST *node)
{
  openType(CppEnumerationType::Ptr(new CppEnumerationType()), node);

  TypeBuilderBase::visitEnumSpecifier(node);

  closeType();
}

void TypeBuilder::visitEnumerator(EnumeratorAST* node)
{
  /*bool ok = false;
  if (CppEnumerationType::Ptr parent = currentType<CppEnumerationType>()) {
    CppEnumeratorType::Ptr enumerator(new CppEnumeratorType());
    openType(enumerator, node);
    ok = true;
  }*/

  TypeBuilderBase::visitEnumerator(node);

  //if (ok)
    //closeType();
}

void TypeBuilder::visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST *node)
{
  AbstractType::Ptr type;

  if (node->name) {
    int kind = m_editor->parseSession()->token_stream->kind(node->type);
    switch (kind) {
      case Token_class:
      case Token_struct:
      case Token_union:
        type = AbstractType::Ptr::staticCast(openClass(kind));
        break;
      case Token_enum:
        type = AbstractType::Ptr(new CppEnumerationType());
        break;
      case Token_typename:
        // TODO what goes here...?
        //type = def->abstractType();
        break;
    }

    if (type)
      openType(type, node);
  }

  // TODO.. figure out what to do with this now... parseConstVolatile(node->cv);

  TypeBuilderBase::visitElaboratedTypeSpecifier(node);

  if (type)
    closeType();
}

void TypeBuilder::visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *node)
{
  bool openedType = false;

  if (node->integrals) {
    CppIntegralType::IntegralTypes type = CppIntegralType::TypeNone;
    CppIntegralType::TypeModifiers modifiers = CppIntegralType::ModifierNone;

    const ListNode<std::size_t> *it = node->integrals->toFront();
    const ListNode<std::size_t> *end = it;
    do {
      int kind = m_editor->parseSession()->token_stream->kind(it->element);
      switch (kind) {
        case Token_char:
          type = CppIntegralType::TypeChar;
          break;
        case Token_wchar_t:
          type = CppIntegralType::TypeWchar_t;
          break;
        case Token_bool:
          type = CppIntegralType::TypeBool;
          break;
        case Token_short:
          modifiers |= CppIntegralType::ModifierShort;
          break;
        case Token_int:
          type = CppIntegralType::TypeInt;
          break;
        case Token_long:
          if (modifiers & CppIntegralType::ModifierLong)
            modifiers |= CppIntegralType::ModifierLongLong;
          else
            modifiers |= CppIntegralType::ModifierLong;
          break;
        case Token_signed:
          modifiers |= CppIntegralType::ModifierSigned;
          break;
        case Token_unsigned:
          modifiers |= CppIntegralType::ModifierUnsigned;
          break;
        case Token_float:
          type = CppIntegralType::TypeFloat;
          break;
        case Token_double:
          type = CppIntegralType::TypeDouble;
          break;
        case Token_void:
          type = CppIntegralType::TypeVoid;
          break;
      }

      it = it->next;
    } while (it != end);

    CppIntegralType::Ptr integral = TypeRepository::self()->integral(type, modifiers, parseConstVolatile(node->cv));
    if (integral) {
      openedType = true;
      openType(integral, node);
    }

  } else if (node->name) {
    /*QualifiedIdentifier id = identifierForName(node->name);
    Cursor pos = m_editor->findPosition(node->start_token, EditorIntegrator::FrontEdge);
    Declaration* dec = currentContext()->findDeclarations(id, pos);
    if (dec && dec->abstractType()) {
      openedType = true;
      openType(dec->abstractType(), node);
    }*/
  }

  TypeBuilderBase::visitSimpleTypeSpecifier(node);

  if (openedType)
    closeType();
}

void TypeBuilder::visitTypedef(TypedefAST* node)
{
  openType(CppTypeAliasType::Ptr(new CppTypeAliasType()), node);

  TypeBuilderBase::visitTypedef(node);

  closeType();
}

void TypeBuilder::visitFunctionDeclaration(FunctionDefinitionAST* node)
{
  m_lastType = 0;

  TypeBuilderBase::visitFunctionDeclaration(node);
}

void TypeBuilder::visitSimpleDeclaration(SimpleDeclarationAST* node)
{
  m_lastType = 0;

  // Reimplement default visitor
  visit(node->type_specifier);

  AbstractType::Ptr baseType = m_lastType;

  if (node->init_declarators) {
    const ListNode<InitDeclaratorAST*> *it = node->init_declarators->toFront(), *end = it;

    do {
      visit(it->element);
      // Reset last type to be the base type
      m_lastType = baseType;

      it = it->next;
    } while (it != end);
  }

  visit(node->win_decl_specifiers);

  visitPostSimpleDeclaration(node);
}

void TypeBuilder::visitPtrOperator(PtrOperatorAST* node)
{
  bool typeOpened = false;
  if (node->op) {
    QString op = m_editor->tokenToString(node->op);
    if (!op.isEmpty())
      if (op[0] == '&') {
        CppReferenceType::Ptr pointer(new CppReferenceType(parseConstVolatile(node->cv)));
        pointer->setBaseType(lastType());
        openType(pointer, node);
        typeOpened = true;

      } else if (op[0] == '*') {
        CppPointerType::Ptr pointer(new CppPointerType(parseConstVolatile(node->cv)));
        pointer->setBaseType(lastType());
        openType(pointer, node);
        typeOpened = true;
      }
  }

  TypeBuilderBase::visitPtrOperator(node);

  if (typeOpened)
    closeType();
}

void TypeBuilder::visitDeclarator(DeclaratorAST *node)
{
  //BEGIN Copied from default visitor
  visit(node->sub_declarator);
  visitNodes(this, node->ptr_ops);
  visit(node->id);
  visit(node->bit_expression);
  //END Finished with first part of default visitor

  // Custom code - create array types
  if (node->array_dimensions) {
    const ListNode<ExpressionAST*> *it = node->array_dimensions->toFront(), *end = it;

    do {
      if (it->element)
        visitArrayExpression(it->element);
      it = it->next;
    } while (it != end);
  }

  if (node->parameter_declaration_clause) {
    // New function type
    CppFunctionType::Ptr newFunction(new CppFunctionType());

    if (node->fun_cv)
      newFunction->setCV(parseConstVolatile(node->fun_cv));

    if (lastType())
      newFunction->setReturnType(lastType());

    openType(newFunction, node);
  }

  //BEGIN Copied from default visitor
  visit(node->parameter_declaration_clause);
  visit(node->exception_spec);
  //END Finished with default visitor

  if (node->parameter_declaration_clause)
    closeType();

  if (lastType() && hasCurrentType())
    if (StructureType::Ptr structure = currentType<StructureType>())
      structure->addElement(lastType());
}

void TypeBuilder::visitArrayExpression(ExpressionAST* expression)
{
  bool typeOpened = false;

  // TODO need generic expression evaluator...
  switch (expression->kind) {
    case AST::Kind_PrimaryExpression: {
      PrimaryExpressionAST* primary = static_cast<PrimaryExpressionAST*>(expression);
      if (primary->token) {
        QString token = m_editor->tokenToString(primary->token);
        bool ok;
        int arrayDimension = token.toInt(&ok);
        if (ok) {
          // Phew...
          ArrayType::Ptr array(new ArrayType());
          array->setElementType(lastType());
          array->setDimension(arrayDimension);
          openType(array, expression);
          typeOpened = true;
        }
        break;
      }
    }
  }

  visit(expression);

  if (typeOpened)
    closeType();
}

AbstractType::Ptr TypeBuilder::lastType() const
{
  return m_lastType;
}

Cpp::CVSpecs TypeBuilder::parseConstVolatile(const ListNode<std::size_t> *cv)
{
  Cpp::CVSpecs ret = Cpp::CVNone;

  if (cv) {
    const ListNode<std::size_t> *it = cv->toFront();
    const ListNode<std::size_t> *end = it;
    do {
      int kind = m_editor->parseSession()->token_stream->kind(it->element);
      if (kind == Token_const)
        ret |= Cpp::Const;
      else if (kind == Token_volatile)
        ret |= Cpp::Volatile;

      it = it->next;
    } while (it != end);
  }

  return ret;
}

// kate: space-indent on; indent-width 2; replace-tabs on;

const QList< AbstractType::Ptr > & TypeBuilder::topTypes() const
{
  return m_topTypes;
}

void TypeBuilder::visitParameterDeclaration(ParameterDeclarationAST* node)
{
  TypeBuilderBase::visitParameterDeclaration(node);

  if (hasCurrentType()) {
    if (CppFunctionType::Ptr function = currentType<CppFunctionType>()) {
      function->addArgument(lastType());
    }
    // else may be a template argument
  }
}
