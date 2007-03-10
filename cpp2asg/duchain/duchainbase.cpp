/* This  is part of KDevelop
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

#include "duchainbase.h"

#include "ast.h"

DUChainBase::DUChainBase(AST *node, Range* range)
  : DocumentRangeObject(range)
  , m_node(node)
  , m_encountered(0)
{
}

DUChainBase::DUChainBase(AST *node)
  : DocumentRangeObject(0)
  , m_node(node)
  , m_encountered(0)
{
}

DUChainBase::~DUChainBase()
{
}

TopDUContext* DUChainBase::topContext() const
{
  return 0;
}

// kate: indent-width 2;
