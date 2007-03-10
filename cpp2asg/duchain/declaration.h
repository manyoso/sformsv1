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

#ifndef DECLARATION_H
#define DECLARATION_H

#include <QList>
#include <QReadWriteLock>

#include "identifier.h"
#include "kdevdocumentrangeobject.h"
#include "cppnamespace.h"
#include "typesystem.h"
#include "duchainbase.h"

class AbstractType;
class DUContext;
class Use;
class Definition;
class ForwardDeclaration;

/**
 * Represents a single declaration in a definition-use chain.
 */
class Declaration : public DUChainBase
{
  friend class DUContext;
  friend class ForwardDeclaration;

public:
  enum Scope {
    GlobalScope,
    NamespaceScope,
    ClassScope,
    FunctionScope,
    LocalScope
  };

  Declaration(AST *node, Range* range, Scope scope, DUContext* context);
  virtual ~Declaration();

  virtual TopDUContext* topContext() const;

  const QList<ForwardDeclaration*>& forwardDeclarations() const;
  virtual bool isForwardDeclaration() const;
  ForwardDeclaration* toForwardDeclaration();
  const ForwardDeclaration* toForwardDeclaration() const;

  bool isDefinition() const;
  void setDeclarationIsDefinition(bool dd);

  /**
   * Retrieve the definition for this use.
   */
  Definition* definition() const;

  /**
   * Set the definition for this use.
   */
  void setDefinition(Definition* definition);

  DUContext* context() const;
  void setContext(DUContext* context);

  Scope scope() const;

  template <class T>
  KSharedPtr<T> type() const { return KSharedPtr<T>::dynamicCast(abstractType()); }

  template <class T>
  void setType(KSharedPtr<T> type) { setAbstractType(AbstractType::Ptr::staticCast(type)); }

  AbstractType::Ptr abstractType() const;
  void setAbstractType(AbstractType::Ptr type);

  void setIdentifier(const Identifier& identifier);
  const Identifier& identifier() const;

  QualifiedIdentifier qualifiedIdentifier() const;

  /**
   * Provides a mangled version of this definition's identifier, for use in a symbol table.
   */
  QString mangledIdentifier() const;

  bool inSymbolTable() const;
  void setInSymbolTable(bool inSymbolTable);

  const QList<Use*>& uses() const;
  void addUse(Use* range);
  void removeUse(Use* range);

  bool operator==(const Declaration& other) const;

  virtual QString toString() const;

private:
  DUContext* m_context;
  Scope m_scope;
  AbstractType::Ptr m_type;
  Identifier m_identifier;

  QList<ForwardDeclaration*> m_forwardDeclarations;

  Definition* m_definition;

  QList<Use*> m_uses;

  bool m_isDefinition  : 1;
  bool m_inSymbolTable : 1;
};

#endif // DECLARATION_H

// kate: indent-width 2;
