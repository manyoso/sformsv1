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

#ifndef CPPEDITORINTEGRATOR_H
#define CPPEDITORINTEGRATOR_H

#include "kdeveditorintegrator.h"
#include "lexer.h"

class AST;

/**
 * Provides facilities for easy integration of a text editor component with
 * the information parsed from a source file.
 *
 * Uses a disguised singleton + stateful design.
 *
 * \todo introduce stacks for the state?
 */
class CppEditorIntegrator : public EditorIntegrator
{
public:
  CppEditorIntegrator(ParseSession* session);

  ParseSession* parseSession() const;

  /**
   * Finds the location and \a file where the given \a token was parsed from.  This function
   * does not change any of the CppEditorIntegrator's state.
   *
   * \param token token to locate
   * \param edge set to FrontEdge to return the start position of the token, BackEdge to return the end position.
   *
   * \returns the requested cursor relating to the start or end of the given token.
   */
  Cursor findPosition(const Token& token, Edge edge = BackEdge) const;

  /**
   * Finds the location and \a file where the given \a token was parsed from.  This function
   * does not change any of the CppEditorIntegrator's state.
   *
   * \param token token to locate
   * \param edge set to FrontEdge to return the start position of the token, BackEdge to return the end position.
   *
   * \returns the requested cursor relating to the start or end of the given token.
   */
  Cursor findPosition(std::size_t token, Edge edge = BackEdge) const;

  using EditorIntegrator::createRange;

  /**
   * Create a new text range encompassing the given AST \a node.
   * The returned range will become the new currentRange().
   *
   * If the current document is loaded, and it supports creating smart ranges,
   * this will be a smart range, otherwise it will be a DocumentRange.
   *
   * \returns the newly created smart range.
   * \overload
   */
  Range findRange(AST* node, RangeEdge = OuterEdge);

  /**
   * Create a new text range encompassing the given AST nodes.
   * The returned range will become the new currentRange().
   *
   * If the current document is loaded, and it supports creating smart ranges,
   * this will be a smart range, otherwise it will be a DocumentRange.
   *
   * \returns the newly created smart range.
   * \overload
   */
  Range findRange(AST* from, AST* to);

  /**
   * Create a new start range encompassing the given AST \a token.
   * The returned range will become the new currentRange().
   *
   * If the current document is loaded, and it supports creating smart ranges,
   * this will be a smart range, otherwise it will be a DocumentRange.
   *
   * \returns the newly created smart range.
   * \overload
   */
  Range findRange(const Token& token);

  /**
   * Create a new start range encompassing the given AST \a token.
   * The returned range will become the new currentRange().
   *
   * If the current document is loaded, and it supports creating smart ranges,
   * this will be a smart range, otherwise it will be a DocumentRange.
   *
   * \returns the newly created smart range.
   * \overload
   */
  Range findRange(std::size_t token);

  /**
   * Retrieve the string represented by a token.
   */
  QString tokenToString(std::size_t token) const;

private:
  ParseSession* m_session;
};

#endif // EDITORINTEGRATOR_H

// kate: indent-width 2;
