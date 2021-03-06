// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SERVICE_SYSTEM_LANGUAGE_H
#define INSTALLER_SERVICE_SYSTEM_LANGUAGE_H

#include <QList>
#include <QString>

namespace installer {

struct LanguageItem {
  QString name;  // English name.
  QString locale;  // Language name, like en_US.
  QString local_name;  // Localized name.
};

typedef QList<LanguageItem> LanguageList;

LanguageList GetLanguageList();

// Generate locale based on |locale|.
// Call `locale-gen` to update system locales, or else setlocale() will fail
// in future.
void GenerateLocale(const QString& locale);

}  // namespace installer

#endif  // INSTALLER_SERVICE_SYSTEM_LANGUAGE_H
