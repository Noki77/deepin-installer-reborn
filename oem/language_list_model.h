// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_OEM_LANGUAGE_LIST_MODEL_H
#define INSTALLER_UI_OEM_LANGUAGE_LIST_MODEL_H

#include <QAbstractListModel>

#include "service/system_language.h"

namespace installer {

// Reimplemented model used in OemWindow.
class LanguageListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit LanguageListModel(QObject* parent = nullptr);

  // If |append_empty| is true, append an empty language item to language list.
  LanguageListModel(bool append_empty, QObject* parent = nullptr);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual int rowCount(const QModelIndex& parent) const override;

  // Get language item at |index|.
  LanguageItem languageItemAt(int index) const;
  LanguageItem languageItemAt(const QModelIndex& index) const;

  // Get index with |locale|.
  QModelIndex localeIndex(const QString& locale) const;

 private:
  LanguageList lang_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_OEM_LANGUAGE_LIST_MODEL_H
