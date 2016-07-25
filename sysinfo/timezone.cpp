// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/timezone.h"

#include <QDebug>
#include <QTextStream>

#include "base/file_util.h"

namespace sysinfo {

QStringList GetZoneInfo() {
  QStringList result;
  QFile file(QStringLiteral("/usr/share/zoneinfo/zone.tab"));
  if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
    qWarning() << "[base]::GetZoneInfo() Failed to open zone.tab!";
    return result;
  }

  QTextStream stream(&file);
  while (!stream.atEnd()) {
    QString line = stream.readLine();
    if (line.startsWith('#')) {
      continue;
    }
    const QStringList line_parts = line.split(QChar('\t'));
    if (line_parts.length() > 3) {
      result.append(line_parts[2]);
    }
  }

  file.close();
  return result;
}

QString GetCurrentTimezone() {
  const QString content =
      base::ReadTextFileContent(QStringLiteral("/etc/timezone"));
  return content.trimmed();
}

}  // namespace sysinfo