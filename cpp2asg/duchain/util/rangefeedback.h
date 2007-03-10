/* This file is part of the KDE project
   Copyright (C) 2003-2005 Hamish Rodda <rodda@kde.org>

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

#ifndef RANGEFEEDBACK_H
#define RANGEFEEDBACK_H

#include <QObject>

#include "attribute.h"

// TODO: Should RangeWatcher become a base class for RangeNotifier?

class Range;
class View;

/**
 * \short A class which provides notifications of state changes to a Range via QObject signals.
 *
 * \ingroup kte_group__classes
 *
 * This class provides notifications of changes to the position or contents of
 * a Range via QObject signals.
 *
 * If you prefer to receive notifications via virtual inheritance, see RangeWatcher.
 *
 * \sa Range, RangeWatcher
 *
 * \author Hamish Rodda \<rodda@kde.org\>
 */
class RangeNotifier : public QObject
{
  Q_OBJECT
  friend class Range;

  public:
    /**
     * Default constructor.
     */
    RangeNotifier();

    /**
     * Returns whether this notifier will notify of changes that happen
     * directly to the range, e.g. by calls to Cursor::setRange(), or by
     * direct assignment to either of the start() or end() cursors, rather
     * than just when surrounding text changes.
     */
    bool wantsDirectChanges() const;

    /**
     * Set whether this notifier should notify of changes that happen
     * directly to the range, e.g. by calls to Cursor::setRange(), or by
     * direct assignment to either of the start() or end() cursors, rather
     * than just when surrounding text changes.
     *
     * \param wantsDirectChanges whether this watcher should provide notifications for direct changes.
     */
    void setWantsDirectChanges(bool wantsDirectChanges);

  Q_SIGNALS:
    /**
     * The range's position changed.
     *
     * \param range pointer to the range which generated the notification.
     */
    void rangePositionChanged(Range* range);

    /**
     * The contents of the range changed.
     *
     * \param range pointer to the range which generated the notification.
     */
    void rangeContentsChanged(Range* range);

    /**
     * The contents of the range changed.  This notification is special in that it is only emitted by
     * the top range of a heirachy, and also gives the furthest descendant child range which still
     * encompasses the whole change (see \p contents).
     *
     * \param range pointer to the range which generated the notification.
     * \param mostSpecificChild the child range which both contains the entire change and is
     *                          the furthest descendant of this range.
     */
    void rangeContentsChanged(Range* range, Range* mostSpecificChild);

    /**
     * The mouse cursor on \a view entered \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    void mouseEnteredRange(Range* range, View* view);

    /**
     * The mouse cursor on \a view exited \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    void mouseExitedRange(Range* range, View* view);

    /**
     * The caret on \a view entered \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    void caretEnteredRange(Range* range, View* view);

    /**
     * The caret on \a view exited \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    void caretExitedRange(Range* range, View* view);

    /**
     * The range now contains no characters (ie. the start and end cursors are the same).
     *
     * \param range pointer to the range which generated the notification.
     */
    void rangeEliminated(Range* range);

    /**
     * The Range instance specified by \p range is being deleted.
     *
     * \param range pointer to the range which is about to be deleted.  It is
     *              still safe to access information at this point.
     */
    void rangeDeleted(Range* range);

    /**
     * The range's parent was changed.
     *
     * \param range pointer to the range which generated the notification.
     * \param newParent pointer to the range which was is now the parent range.
     * \param oldParent pointer to the range which used to be the parent range.
     */
    void parentRangeChanged(Range* range, Range* newParent, Range* oldParent);

    /**
     * The range \a child was inserted as a child range into the current \a range.
     *
     * \param range pointer to the range which generated the notification.
     * \param child pointer to the range which was inserted as a child range.
     */
    void childRangeInserted(Range* range, Range* child);

    /**
     * The child range \a child was removed from the current \a range.
     *
     * \param range pointer to the range which generated the notification.
     * \param child pointer to the child range which was removed.
     */
    void childRangeRemoved(Range* range, Range* child);

    /**
     * The highlighting attribute of \a range was changed from \a previousAttribute to
     * \a currentAttribute.
     *
     * \param range pointer to the range which generated the notification.
     * \param currentAttribute the attribute newly assigned to this range
     * \param previousAttribute the attribute previously assigned to this range
     */
    void rangeAttributeChanged(Range* range, Attribute::Ptr currentAttribute, Attribute::Ptr previousAttribute);

  private:
    bool m_wantDirectChanges;
};

/**
 * \short A class which provides notifications of state changes to a Range via virtual inheritance.
 *
 * \ingroup kte_group__classes
 *
 * This class provides notifications of changes to the position or contents of
 * a Range via virtual inheritance.
 *
 * If you prefer to receive notifications via QObject signals, see RangeNotifier.
 *
 * \sa Range, RangeNotifier
 *
 * \author Hamish Rodda \<rodda@kde.org\>
 */
class RangeWatcher
{
  public:
    /**
     * Default constructor
     */
    RangeWatcher();

    /**
     * Virtual destructor
     */
    virtual ~RangeWatcher();

    /**
     * Returns whether this watcher will be notified of changes that happen
     * directly to the range, e.g. by calls to Cursor::setRange(), or by
     * direct assignment to either of the start() or end() cursors, rather
     * than just when surrounding text changes.
     */
    bool wantsDirectChanges() const;

    /**
     * Set whether this watcher should be notified of changes that happen
     * directly to the range, e.g. by calls to Cursor::setRange(), or by
     * direct assignment to either of the start() or end() cursors, rather
     * than just when surrounding text changes.
     *
     * \param wantsDirectChanges whether this watcher should receive notifications for direct changes.
     */
    void setWantsDirectChanges(bool wantsDirectChanges);

    /**
     * The range's position changed.
     *
     * \param range pointer to the range which generated the notification.
     */
    virtual void rangePositionChanged(Range* range);

    /**
     * The contents of the range changed.
     *
     * \param range pointer to the range which generated the notification.
     */
    virtual void rangeContentsChanged(Range* range);

    /**
     * The contents of the range changed.  This notification is special in that it is only emitted by
     * the top range of a heirachy, and also gives the furthest descendant child range which still
     * encompasses the whole change (see \p contents).
     *
     * \param range the range which has changed
     * \param mostSpecificChild the child range which both contains the entire change and is 
     *                          the furthest descendant of this range.
     */
    virtual void rangeContentsChanged(Range* range, Range* mostSpecificChild);

    /**
     * The mouse cursor on \a view entered \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    virtual void mouseEnteredRange(Range* range, View* view);

    /**
     * The mouse cursor on \a view exited \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    virtual void mouseExitedRange(Range* range, View* view);

    /**
     * The caret on \a view entered \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    virtual void caretEnteredRange(Range* range, View* view);

    /**
     * The caret on \a view exited \p range.
     *
     * \todo For now, to receive this notification, the range heirachy must be registered with
     * the Interface as for arbitrary highlighting with dynamic highlighting.
     * Need to add another (and probably simplify existing) method.
     *
     * \param range pointer to the range which generated the notification.
     * \param view view over which the mouse moved to generate the notification
     */
    virtual void caretExitedRange(Range* range, View* view);

    /**
     * The range now contains no characters (ie. the start and end cursors are the same).
     *
     * \param range pointer to the range which generated the notification.
     */
    virtual void rangeEliminated(Range* range);

    /**
     * The Range instance specified by \p range is being deleted.
     *
     * \param range pointer to the range which is about to be deleted.  It is
     *              still safe to access information at this point.
     */
    virtual void rangeDeleted(Range* range);

    /**
     * The range's parent was changed.
     *
     * \param range pointer to the range which generated the notification.
     * \param newParent pointer to the range which was is now the parent range.
     * \param oldParent pointer to the range which used to be the parent range.
     */
    virtual void parentRangeChanged(Range* range, Range* newParent, Range* oldParent);

    /**
     * The range \a child was inserted as a child range into the current \a range.
     *
     * \param range pointer to the range which generated the notification.
     * \param child pointer to the range which was inserted as a child range.
     */
    virtual void childRangeInserted(Range* range, Range* child);

    /**
     * The child range \a child was removed from the current \a range.
     *
     * \param range pointer to the range which generated the notification.
     * \param child pointer to the child range which was removed.
     */
    virtual void childRangeRemoved(Range* range, Range* child);

    /**
     * The highlighting attribute of \a range was changed from \a previousAttribute to
     * \a currentAttribute.
     *
     * \param range pointer to the range which generated the notification.
     * \param currentAttribute the attribute newly assigned to this range
     * \param previousAttribute the attribute previously assigned to this range
     */
    virtual void rangeAttributeChanged(Range* range, Attribute::Ptr currentAttribute, Attribute::Ptr previousAttribute);

  private:
    bool m_wantDirectChanges;
};

#endif

// kate: space-indent on; indent-width 2; replace-tabs on;
