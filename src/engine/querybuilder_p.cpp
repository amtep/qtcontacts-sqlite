/*
 * Copyright (C) 2014 Jolla Ltd.
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "querybuilder_p.h"

#include <QDebug>

#include "trace_p.h"

QueryBuilder::QueryBuilder(const QString & table)
    : m_valid(true), m_distinct(false), m_limit(0)
{
    m_mainTable = table;
}

QueryBuilder::~QueryBuilder()
{
}

QString QueryBuilder::toString() const
{
    if (!m_valid)
        return QString();
    QStringList pieces;
    pieces << QStringLiteral("SELECT");
    if (m_distinct)
        pieces << QStringLiteral("DISTINCT");
    if (m_queryFields.isEmpty())
        pieces << QStringLiteral("1");
    else
        pieces << m_queryFields.join(QStringLiteral(", "));
    pieces << QStringLiteral("FROM");
    pieces << m_mainTable;
    pieces.append(m_joins);
    if (!m_conditions.isEmpty()) {
        pieces << QStringLiteral("WHERE");
        pieces << m_conditions.join(QStringLiteral(" AND "));
    }
    if (!m_orderBy.isEmpty()) {
        pieces << QStringLiteral("ORDER BY");
        pieces << m_orderBy.join(QStringLiteral(", "));
    }
    if (m_limit > 0) {
        pieces << QStringLiteral("LIMIT");
        pieces << QString::number(m_limit);
    }
    return pieces.join(QChar(' '));
}

void QueryBuilder::setDistinct(bool distinct)
{
    m_distinct = distinct;
}

void QueryBuilder::queryField(const QString & table, const QString & column)
{
    static const QString templ("%1.%2");
    m_queryFields << templ.arg(table).arg(column);
}

void QueryBuilder::leftJoinUsing(const QString & table, const QString & column)
{
    static const QString templ("LEFT JOIN %1 USING (%2)");
    QString join = templ.arg(table).arg(column);
    if (!m_joins.contains(join))
        m_joins << join;
}

void QueryBuilder::andWhere(const QString & condition)
{
    m_conditions << condition;
}

void QueryBuilder::orderBy(const QString & ordering, Qt::SortOrder direction, const QString & collation)
{
    QString expr = ordering;
    if (!collation.isEmpty()) {
        expr.append(QStringLiteral(" COLLATE "));
        expr.append(collation);
    }
    if (direction == Qt::DescendingOrder)
        expr.append(QStringLiteral(" DESC"));

    m_orderBy << expr;
}

void QueryBuilder::setLimit(int limit)
{
    m_limit = limit;
}
