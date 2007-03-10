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

#ifndef DECLARATIONBUILDER_H
#define DECLARATIONBUILDER_H

#include "typebuilder.h"
#include "cppnamespace.h"
#include "classfunctiondeclaration.h"

class Declaration;

typedef TypeBuilder DeclarationBuilderBase;

/**
 * A class which iterates the AST to extract definitions of types.
 */
class DeclarationBuilder: public DeclarationBuilderBase
{
public:
  DeclarationBuilder(ParseSession* session);
  DeclarationBuilder(CppEditorIntegrator* editor);

  /**
   * Compile either a context-definition chain, or add uses to an existing
   * chain.
   *
   * \param includes contexts to reference from the top context.  The list may be changed by this function.
   */
  TopDUContext* buildDeclarations(const QUrl& url, AST *node, QList<DUContext*>* includes = 0);

protected:
  virtual void openContext(DUContext* newContext);
  virtual void closeContext();

  virtual void visitDeclarator (DeclaratorAST*);
  virtual void visitClassSpecifier(ClassSpecifierAST*);
  virtual void visitAccessSpecifier(AccessSpecifierAST*);
  virtual void visitFunctionDeclaration(FunctionDefinitionAST*);
  virtual void visitSimpleDeclaration(SimpleDeclarationAST*);
  virtual void visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST*);

private:
  ForwardDeclaration* openForwardDeclaration(NameAST* name, AST* range);
  /**
   * Register a new declaration with the definition-use chain.
   * Returns the new context created by this definition.
   * \param range provide a valid AST here if name is null
   */
  Declaration* openDeclaration(NameAST* name, AST* range, bool isFunction = false, bool isForward = false, bool isDefinition = false);
  /// Same as the above, but sets it as the definition too
  Declaration* openDefinition(NameAST* name, AST* range, bool isFunction = false);
  void closeDeclaration();
  void abortDeclaration();

  void parseStorageSpecifiers(const ListNode<std::size_t>* storage_specifiers);
  void parseFunctionSpecifiers(const ListNode<std::size_t>* function_specifiers);

  inline bool hasCurrentDeclaration() const { return !m_declarationStack.isEmpty(); }
  inline Declaration* currentDeclaration() const { return m_declarationStack.top(); }

  inline Cpp::AccessPolicy currentAccessPolicy() { return m_accessPolicyStack.top(); }
  inline void setAccessPolicy(Cpp::AccessPolicy policy) { m_accessPolicyStack.top() = policy; }

  inline int& nextDeclaration() { return m_nextDeclarationStack.top(); }

  void applyStorageSpecifiers();
  void applyFunctionSpecifiers();
  void popSpecifiers();

  QStack<Declaration*> m_declarationStack;
  QStack<Cpp::AccessPolicy> m_accessPolicyStack;
  QStack<int> m_nextDeclarationStack;

  QStack<ClassFunctionDeclaration::FunctionSpecifiers> m_functionSpecifiers;
  QStack<ClassMemberDeclaration::StorageSpecifiers> m_storageSpecifiers;
  QStack<std::size_t> m_functionDefinedStack;
};

#endif // DECLARATIONBUILDER_H

// kate: indent-width 2;
