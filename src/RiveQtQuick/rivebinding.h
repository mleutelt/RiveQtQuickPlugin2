#pragma once

#include <QObject>
#include <QVariant>
#include <QtQml/qqmlengine.h>

class RiveBinding : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
  Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged FINAL)
  QML_ELEMENT

  public:
  explicit RiveBinding(QObject* parent = nullptr);

  QString path() const;
  void setPath(const QString& path);

  QVariant value() const;
  void setValue(const QVariant& value);

  signals:
  void pathChanged();
  void valueChanged();

  private:
  QString m_path;
  QVariant m_value;
};
