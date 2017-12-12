/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#ifndef CRYINCLUDE_EDITOR_STRINGDLG_H
#define CRYINCLUDE_EDITOR_STRINGDLG_H

#pragma once

#include <QInputDialog>
#include <QMessageBox>

/////////////////////////////////////////////////////////////////////////////
// StringDlg Qt dialog

// return false if error input
typedef bool (StringDlgPredicate)(QString input);

class StringDlg : public QInputDialog
{
public:
    StringDlg(const QString &title, QWidget* pParent = NULL, bool bFileNameLimitation = false);

    void SetCheckCallback(const std::function<StringDlgPredicate>& Check) {
        m_Check = Check;
    }
    void SetString(const QString &str) {
        setTextValue(str);
    }
    QString GetString() {
        return textValue();
    }

protected:
    virtual void accept();
    bool m_bFileNameLimitation;
    std::function<StringDlgPredicate> m_Check;
};


/////////////////////////////////////////////////////////////////////////////
// StringGroupDlg Qt dialog
class SANDBOX_API StringGroupDlg : public QDialog
{
    // Construction
public:
    StringGroupDlg(const QString &title = QString(), QWidget *parent = 0);

    void SetString(const QString &str);
    QString GetString() const;

    void SetGroup(const QString &str);
    QString GetGroup() const;

protected:
    QLineEdit *m_string;
    QLineEdit *m_group;
};


#endif // CRYINCLUDE_EDITOR_STRINGDLG_H