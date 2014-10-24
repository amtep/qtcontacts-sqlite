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

#ifndef QTCONTACTSSQLITE_QUERYBUILDER_P
#define QTCONTACTSSQLITE_QUERYBUILDER_P

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantList>

namespace QtContactsSqlitePrivate {

// Helper for building the complicated SELECT queries needed
// for the contacts database.
class QueryBuilder {
public:
    QueryBuilder(const QString & table);
    ~QueryBuilder();

    bool isValid() const
        { return m_valid; }
    QString toString() const;

    void setDistinct(bool distinct);
    void queryField(const QString & table, const QString & column);
    // the querybuilder will filter out duplicate joins
    void joinUsing(const QString & table, const QString & column);
    void andWhere(const QString & condition);

private:
    bool m_valid;
    bool m_distinct;
    QString m_mainTable;
    QStringList m_queryFields;
    QStringList m_joins;
    QStringList m_conditions;
};

} // namespace QtContactsSqlitePrivate

#endif
