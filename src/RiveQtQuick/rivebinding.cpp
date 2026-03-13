#include "rivebinding.h"

RiveBinding::RiveBinding(QObject* parent)
    : QObject(parent)
{
}

QString RiveBinding::path() const
{
  return m_path;
}

void RiveBinding::setPath(const QString& path)
{
  if (m_path == path) {
    return;
  }
  m_path = path;
  emit pathChanged();
}

QVariant RiveBinding::value() const
{
  return m_value;
}

void RiveBinding::setValue(const QVariant& value)
{
  if (m_value == value) {
    return;
  }
  m_value = value;
  emit valueChanged();
}
