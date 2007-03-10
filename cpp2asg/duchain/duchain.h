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

#ifndef DUCHAIN_H
#define DUCHAIN_H

#include <QObject>

#include <QUrl>
#include <QMap>

#include "duchainobserver.h"

class TopDUContext;
class QReadWriteLock;

/**
 * Holds references to all top level source file contexts.
 *
 * \todo to pull the sorting off properly, will need to know the location of
 *       the defines used to pull in URLs other than the source file URL.
 */
class DUChain : public QObject
{
  Q_OBJECT

  template <typename T>
  friend class KStaticDeleter;

public:
  TopDUContext* chainForDocument(const QUrl& document);
  void addDocumentChain(const QUrl& document, TopDUContext* chain);

  void clear();

  static DUChain* self();

  /**
   * Retrieve the read write lock for the entire definition-use chain.
   * To call non-const methods, you must be holding a write lock.
   *
   * Evaluations made prior to holding a lock (including which objects
   * exist) must be verified once the lock is held, as they may have changed
   * or been deleted.
   *
   * \threadsafe
   */
  inline static QReadWriteLock* lock() { return s_lock; }

  const QList<DUChainObserver*>& observers() const;
  void addObserver(DUChainObserver* observer);
  void removeObserver(DUChainObserver* observer);

  // Distribute the notifications
  static void contextChanged(DUContext* context, DUChainObserver::Modification change, DUChainObserver::Relationship relationship, DUChainBase* relatedObject = 0);
  static void declarationChanged(Declaration* declaration, DUChainObserver::Modification change, DUChainObserver::Relationship relationship, DUChainBase* relatedObject = 0);
  static void definitionChanged(Definition* definition, DUChainObserver::Modification change, DUChainObserver::Relationship relationship, DUChainBase* relatedObject = 0);
  static void useChanged(Use* use, DUChainObserver::Modification change, DUChainObserver::Relationship relationship, DUChainBase* relatedObject = 0);

public slots:
  void removeDocumentChain(const QUrl& document);

private:
  DUChain();
  ~DUChain();

  static DUChain* s_chain;
  static QReadWriteLock* s_lock;
  QMap<QUrl, TopDUContext*> m_chains;

  QList<DUChainObserver*> m_observers;
};

#endif // DUCHAIN_H

// kate: indent-width 2;
