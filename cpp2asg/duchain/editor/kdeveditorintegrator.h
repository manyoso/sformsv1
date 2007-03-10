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

#ifndef KDEVEDITORINTEGRATOR_H
#define KDEVEDITORINTEGRATOR_H

#include <QUrl>

#include "range.h"

#include "kdevdocumentcursor.h"

class QMutex;

class DocumentRange;
class DocumentCursor;
class EditorIntegratorPrivate;

/**
 * Provides facilities for easy integration of a text editor component with
 * the information parsed from a source file.
 *
 * Uses a disguised singleton + stateful design.
 *
 * \todo introduce stacks for the state?
 *
 * \todo non-loaded documents don't trigger the removeDocument call...
 */
class EditorIntegrator
{
public:
  EditorIntegrator();
  virtual ~EditorIntegrator();

  /**
   * Initialise the editor integrator.
   * Only needs to be called once from the main thread.
   */
  static void initialise();

  const QUrl& currentUrl() const;
  void setCurrentUrl(const QUrl& currentUrl);

  enum TopRangeType {
    Highlighting,
    DefinitionUseChain,

    TopRangeCount
  };

  /**
   * Returns a toplevel range in \a document for use as \a type.
   *
   * \param type The use for which the created range will be used
   * \param document The text editor document to create in.  If one is not
   *                 defined, the currently active document will be used
   *                 instead.
   * \returns the range either found or created, if the document was valid and supports
   *          smart ranges.
   */
  Range* topRange(TopRangeType type);

  /**
   * Releases a toplevel \a range.  The range should be deleted by the caller.
   */
  static void releaseTopRange(Range* range);

  /**
   * Releases + safely deletes a text editor range.
   */
  static void releaseRange(Range* range);

  enum Edge {
    FrontEdge,
    BackEdge
  };

  /**
   * Create a new persistant cursor from the given \a position.
   */
  Cursor* createCursor(const Cursor& position);

  /**
   * Create a new persistant cursor from the given \a token on the given \a edge.
   */
  Cursor* createCursor(std::size_t token, Edge edge);

  // Set defaults for creation of ranges
  void setNewRange(const Range& range);
  void setNewStart(const Cursor& position);
  void setNewEnd(const Cursor& position);

  /**
   * Create a text range over \a range as a child range of the current range.
   * The returned range will become the new currentRange().
   *
   * If the current document is loaded, and it supports creating smart ranges,
   * this will be a smart range, otherwise it will be a DocumentRange.
   *
   * \param range Range of text to cover.  If this is outside the parent's range, the
   * parent will be adjusted (standard behaviour of SmartRange%s).
   *
   * \returns the newly created text range.
   */
  Range* createRange(const Range& range);

  /**
   * Create a text range from \a start to \a end as a child range of the current range.
   * The returned range will become the new currentRange().
   *
   * If the current document is loaded, and it supports creating smart ranges,
   * this will be a smart range, otherwise it will be a DocumentRange.
   *
   * \param start start of the range of text to cover.  If this is outside the parent's range, the
   * parent will be adjusted (standard behaviour of SmartRange%s).
   * \param end end of the range of text to cover.  If this is outside the parent's range, the
   * parent will be adjusted (standard behaviour of SmartRange%s).
   *
   * \returns the newly created text range.
   * \overload
   */
  Range* createRange(const Cursor& start, const Cursor& end);

  /**
   * Create a text range over the marked range as a child range of the current range.
   * The returned range will become the new currentRange().
   *
   * If the current document is loaded, and it supports creating smart ranges,
   * this will be a smart range, otherwise it will be a DocumentRange.
   *
   * \returns the newly created smart range.
   * \overload
   */
  Range* createRange();

  enum RangeEdge {
    InnerEdge,
    OuterEdge
  };

  /**
   * Returns the current text range.
   */
  Range* currentRange() const;

  /**
   * Sets the current range to \a range.
   */
  void setCurrentRange(Range* range);

  /**
   * Sets the parent range to be the new current range.
   */
  void exitCurrentRange();

protected:
  static EditorIntegratorPrivate* data();

  static EditorIntegratorPrivate* s_data;

  QUrl m_currentUrl;
  Range* m_currentRange;
  Range m_newRangeMarker;
};

#endif // EDITORINTEGRATOR_H

// kate: indent-width 2;
