// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ITSAMEME_QT_ITSAMEMEADDRESSVALIDATOR_H
#define ITSAMEME_QT_ITSAMEMEADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ITSAMEMEAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ITSAMEMEAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** ITSAMEME address widget validator, checks for a valid itsameme address.
 */
class ITSAMEMEAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ITSAMEMEAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // ITSAMEME_QT_ITSAMEMEADDRESSVALIDATOR_H
