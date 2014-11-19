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

#define QT_STATICPLUGIN

#include "../../util.h"
#include "querybuilder_p.h"

class tst_QueryBuilder : public QObject
{
    Q_OBJECT

public:
    tst_QueryBuilder();
    virtual ~tst_QueryBuilder();

public slots:
    void init();
    void cleanup();

private slots:
    void emptyFields();
    void oneField();
    void charField();
    void multiField();
    void distinct();
    void joinUsing();
    void joinUsingDuplicate();
    void whereClause();
    void whereClauses();
    void ordering();
    void orderingCollation();
    void orderingDesc();
    void orderings();
    void withLimit();
    void withNoLimit();

private:
    QueryBuilder *qb;
};

tst_QueryBuilder::tst_QueryBuilder()
    : qb(0)
{
}

tst_QueryBuilder::~tst_QueryBuilder()
{
}

void tst_QueryBuilder::init()
{
    qb = new QueryBuilder("Contacts");
}

void tst_QueryBuilder::cleanup()
{
    delete qb;
    qb = 0;
}

void tst_QueryBuilder::emptyFields()
{
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::oneField()
{
    qb->queryField(QString::fromLatin1("Contacts"), QString::fromLatin1("firstName"));
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT Contacts.firstName FROM Contacts");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::charField()
{
    qb->queryField("Contacts", "firstName");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT Contacts.firstName FROM Contacts");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::multiField()
{
    qb->queryField("Contacts", "firstName");
    qb->queryField("Contacts", "lastName");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT Contacts.firstName, Contacts.lastName FROM Contacts");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::distinct()
{
    qb->setDistinct(true);
    qb->queryField("Contacts", "firstName");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT DISTINCT Contacts.firstName FROM Contacts");
    QCOMPARE(qb->toString(), expected);

    qb->queryField("Contacts", "lastName");
    QVERIFY(qb->isValid());
    expected = QString::fromLatin1("SELECT DISTINCT Contacts.firstName, Contacts.lastName FROM Contacts");
    QCOMPARE(qb->toString(), expected);

    qb->setDistinct(false);
    QVERIFY(qb->isValid());
    expected = QString::fromLatin1("SELECT Contacts.firstName, Contacts.lastName FROM Contacts");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::joinUsing()
{
    qb->leftJoinUsing("PhoneNumbers", "contactId");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts LEFT JOIN PhoneNumbers USING (contactId)");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::joinUsingDuplicate()
{
    qb->leftJoinUsing("PhoneNumbers", "contactId");
    qb->leftJoinUsing("OnlineAccounts", "contactId");
    qb->leftJoinUsing("PhoneNumbers", "contactId");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts LEFT JOIN PhoneNumbers USING (contactId) LEFT JOIN OnlineAccounts USING (contactId)");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::whereClause()
{
    qb->andWhere("Contacts.isDeactivated = 0");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts WHERE Contacts.isDeactivated = 0");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::whereClauses()
{
    qb->andWhere("Contacts.isDeactivated = 0");
    qb->andWhere("Contacts.contactId > 2");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts WHERE Contacts.isDeactivated = 0 AND Contacts.contactId > 2");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::ordering()
{
    qb->orderBy("Contacts.contactId");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts ORDER BY Contacts.contactId");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::orderingCollation()
{
    qb->orderBy("Contacts.lastName", Qt::AscendingOrder, "NOCASE");
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts ORDER BY Contacts.lastName COLLATE NOCASE");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::orderingDesc()
{
    qb->orderBy("Contacts.contactId", Qt::DescendingOrder);
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts ORDER BY Contacts.contactId DESC");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::orderings()
{
    qb->orderBy("Contacts.hasPhoneNumber");
    qb->orderBy("Contacts.contactId", Qt::DescendingOrder);
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts ORDER BY Contacts.hasPhoneNumber, Contacts.contactId DESC");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::withLimit()
{
    qb->setLimit(20);
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts LIMIT 20");
    QCOMPARE(qb->toString(), expected);
}

void tst_QueryBuilder::withNoLimit()
{
    qb->setLimit(-1);
    QVERIFY(qb->isValid());
    QString expected = QString::fromLatin1("SELECT 1 FROM Contacts");
    QCOMPARE(qb->toString(), expected);
}

QTEST_GUILESS_MAIN(tst_QueryBuilder)
#include "tst_querybuilder.moc"
