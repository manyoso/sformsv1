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

#include "kdeveditorintegrator.h"
#include "kdeveditorintegrator_p.h"

#include <limits.h>

#include <QMutex>
#include <QMutexLocker>

#include "kdevdocumentrange.h"
#include "kdevdocumentrangeobject.h"

EditorIntegratorPrivate* EditorIntegrator::s_data = 0;

EditorIntegrator::EditorIntegrator()
  : m_currentRange(0)
{
}

EditorIntegrator::~ EditorIntegrator()
{
}

EditorIntegratorPrivate::EditorIntegratorPrivate()
  : mutex(new QMutex)
{
}

EditorIntegratorPrivate::~EditorIntegratorPrivate()
{
  QMapIterator<QUrl, QVector<Range*> > it = topRanges;
  while (it.hasNext()) {
    it.next();
  }

  delete mutex;
}

Cursor* EditorIntegrator::createCursor(const Cursor& position)
{
  Cursor* ret = 0;

  if (!ret)
    ret = new DocumentCursor(m_currentUrl, position);

  return ret;
}

Range* EditorIntegrator::topRange( TopRangeType type )
{
  QMutexLocker lock(data()->mutex);

  if (!data()->topRanges.contains(currentUrl()))
    data()->topRanges.insert(currentUrl(), QVector<Range*>(TopRangeCount));

  if (!data()->topRanges[currentUrl()][type]) {
    data()->topRanges[currentUrl()][type] = createRange(Range(0,0, INT_MAX, INT_MAX));
  }

  m_currentRange = data()->topRanges[currentUrl()][type];
  return m_currentRange;
}

void EditorIntegratorPrivate::rangeDeleted(Range * range)
{
  QMutexLocker lock(mutex);

  QMutableMapIterator<QUrl, QVector<Range*> > it = topRanges;
  while (it.hasNext()) {
    it.next();
    //qDebug() << "Searching for " << range << ", potentials " << it.value().toList() << endl;
    int index = it.value().indexOf(range);
    if (index != -1) {
      it.value()[index] = 0;
      return;
    }
  }

  // Should have found the top level range by now
  qWarning() << "Could not find record of top level range " << range << "!" << endl;
}

Range* EditorIntegrator::createRange( const Range & range )
{
  m_currentRange = new DocumentRange(m_currentUrl, range, m_currentRange);
  return m_currentRange;
}


Range* EditorIntegrator::createRange( const Cursor& start, const Cursor& end )
{
  return createRange(Range(start, end));
}

Range* EditorIntegrator::createRange()
{
  return createRange(m_newRangeMarker);
}

void EditorIntegrator::setNewRange(const Range& range)
{
  m_newRangeMarker = range;
}

void EditorIntegrator::setNewEnd( const Cursor & position )
{
  m_newRangeMarker.end() = position;
}

void EditorIntegrator::setNewStart( const Cursor & position )
{
  m_newRangeMarker.start() = position;
}

void EditorIntegrator::setCurrentRange( Range* range )
{
  m_currentRange = range;
}

Range* EditorIntegrator::currentRange( ) const
{
  return m_currentRange;
}

const QUrl& EditorIntegrator::currentUrl() const
{
  return m_currentUrl;
}

void EditorIntegrator::setCurrentUrl(const QUrl& url)
{
  m_currentUrl = url;
}

void EditorIntegrator::releaseTopRange(Range * range)
{
  QMutexLocker lock(data()->mutex);

  QUrl url = DocumentRangeObject::url(range);

  if (data()->topRanges.contains(url)) {
    QVector<Range*>& ranges = data()->topRanges[url];
    int index = ranges.indexOf(range);
    if (index != -1) {
      ranges[index] = 0;
      return;
    }
  }

  //kWarning() << k_funcinfo << "Could not find top range to delete." << endl;
}

void EditorIntegrator::releaseRange(Range* range)
{
  if (range) {
    delete range;
  }
}

EditorIntegratorPrivate * EditorIntegrator::data()
{
  if (!s_data)
    s_data = new EditorIntegratorPrivate();

  return s_data;
}

void EditorIntegrator::exitCurrentRange()
{
  if (!m_currentRange)
    return;

  m_currentRange = static_cast<DocumentRange*>(m_currentRange)->parentRange();
}

void EditorIntegrator::initialise()
{
  data();
}

// kate: indent-width 2;
