/* This file is part of KDevelop
    Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
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

#include "dumpchain.h"

#include <QtCore/QString>

#include <QDebug>

#include "cppeditorintegrator.h"
#include "ducontext.h"
#include "declaration.h"
#include "definition.h"
#include "use.h"

static char const * const names[] = {
  0,
  "AccessSpecifier",
  "AsmDeclaration",
  "BaseClause",
  "BaseSpecifier",
  "BinaryExpression",
  "CastExpression",
  "ClassMemberAccess",
  "ClassSpecifier",
  "CompoundStatement",
  "Condition",
  "ConditionalExpression",
  "CppCastExpression",
  "CtorInitializer",
  "DeclarationStatement",
  "Declarator",
  "DeleteExpression",
  "DoStatement",
  "ElaboratedTypeSpecifier",
  "EnumSpecifier",
  "Enumerator",
  "ExceptionSpecification",
  "ExpressionOrDeclarationStatement",
  "ExpressionStatement",
  "ForStatement",
  "FunctionCall",
  "FunctionDeclaration",
  "IfStatement",
  "IncrDecrExpression",
  "InitDeclarator",
  "Initializer",
  "InitializerClause",
  "LabeledStatement",
  "LinkageBody",
  "LinkageSpecification",
  "MemInitializer",
  "Name",
  "Namespace",
  "NamespaceAliasDeclaration",
  "NewDeclarator",
  "NewExpression",
  "NewInitializer",
  "NewTypeId",
  "Operator",
  "OperatorFunctionId",
  "ParameterDeclaration",
  "ParameterDeclarationClause",
  "PostfixExpression",
  "PrimaryExpression",
  "PtrOperator",
  "PtrToMember",
  "ReturnStatement",
  "SimpleDeclaration",
  "SimpleTypeSpecifier",
  "SizeofExpression",
  "StringLiteral",
  "SubscriptExpression",
  "SwitchStatement",
  "TemplateArgument",
  "TemplateDeclaration",
  "TemplateParameter",
  "ThrowExpression",
  "TranslationUnit",
  "TryBlockStatement",
  "TypeId",
  "TypeIdentification",
  "TypeParameter",
  "Typedef",
  "UnaryExpression",
  "UnqualifiedName",
  "Using",
  "UsingDirective",
  "WhileStatement",
  "WinDeclSpec"
};

DumpChain::DumpChain()
  : m_editor(0)
  , indent(0)
{
}

void DumpChain::dump( AST * node, ParseSession* session)
{
  delete m_editor;
  m_editor = 0;

  if (session)
    m_editor = new CppEditorIntegrator(session);

  visit(node);
}

void DumpChain::visit(AST *node)
{
  if (node)
    if (m_editor)
      qDebug() << QString(indent * 2, ' ').toLatin1().constData()
               << names[node->kind]
               << '[' << m_editor->findPosition(node->start_token, CppEditorIntegrator::FrontEdge)
               << ", "
               << m_editor->findPosition(node->end_token, CppEditorIntegrator::FrontEdge)
               << ']'
               << endl;
    else
      qDebug() << QString(indent * 2, ' ').toLatin1().constData()
               << names[node->kind]
               << '[' << node->start_token
               << ", " << node->end_token
               << ']'
               << endl;

  ++indent;
  DefaultVisitor::visit(node);
  --indent;

  if (node)
    if (m_editor)
      qDebug() << QString(indent * 2, ' ').toLatin1().constData()
               << names[node->kind]
               << "[Close: "
               << m_editor->findPosition(node->start_token, CppEditorIntegrator::FrontEdge)
               << ", "
               << m_editor->findPosition(node->end_token, CppEditorIntegrator::FrontEdge)
               << ']'
               << endl;
    else
      qDebug() << QString(indent * 2, ' ').toLatin1().constData()
               << names[node->kind]
               << "[Close: "
               << node->start_token
               << ", "
               << node->end_token
               << ']'
               << endl;
}

DumpChain::~ DumpChain( )
{
  delete m_editor;
}

void DumpChain::dump( DUContext * context, bool imported )
{
  qDebug().nospace() << QString(indent * 2, ' ').toLatin1().constData()
           << (imported ? "==import==> Context " : "New Context ")
           << context->localScopeIdentifier()
           << " ["
           << context->scopeIdentifier()
           << "] "
           << context->textRange()
           << " in "
           << context->url().toLocalFile()
           << endl;

  if (!imported) {
    foreach (Declaration* dec, context->localDeclarations()) {
      qDebug().nospace() << QString((indent+1) * 2, ' ').toLatin1().constData()
               << dec->toString().toLatin1().constData()
               << " ["
               << dec->qualifiedIdentifier()
               << "]  "
               << dec->textRange()
               << ", "
               << (dec->isDefinition() ? "defined, " : (dec->definition() ? "definition to follow, " : "no definition, "))
               << dec->uses().count()
               << " use(s) in "
               << dec->url().toLocalFile()
               << endl;

      if (dec->definition())
        qDebug().nospace() << QString((indent+1) * 2 + 1, ' ').toLatin1().constData()
                 << "Definition: "
                 << dec->definition()->textRange()
                 << " in "
                 << dec->url().toLocalFile()
                 << endl;

      foreach (Use* use, dec->uses())
        qDebug().nospace() << QString((indent+2) * 2, ' ').toLatin1().constData()
                 << "Use: "
                 << use->textRange()
                 << " in "
                 << use->url().toLocalFile()
                 << endl;
    }
  }

  ++indent;
  if (!imported) {
    foreach (DUContext* parent, context->importedParentContexts()) {
      dump(parent, true);
    }

    foreach (DUContext* child, context->childContexts())
      dump(child);
  }
  --indent;
}

// kate: space-indent on; indent-width 2; replace-tabs on;
