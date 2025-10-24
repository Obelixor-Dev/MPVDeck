#ifndef MPVDECK_CONTROLFACTORY_H
#define MPVDECK_CONTROLFACTORY_H

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class SettingsViewModel;

namespace ControlFactory
{

template <typename Signal>
QWidget* createCheckBox(const QString& label, bool checked,
                        const QString&                   toolTip,
                        const std::function<void(bool)>& setter,
                        const QObject* receiver, Signal signal, QWidget* parent)
{
  auto* checkBox = new QCheckBox(label, parent);
  checkBox->setChecked(checked);
  checkBox->setToolTip(toolTip);
  QObject::connect(checkBox, &QCheckBox::toggled, setter);
  QObject::connect(
      static_cast<const typename QtPrivate::FunctionPointer<Signal>::Object*>(
          receiver),
      signal, checkBox, &QCheckBox::setChecked);
  return checkBox;
}

template <typename Signal>
QWidget* createSlider(const QString& label, int value, int min, int max,
                      const QString&                  toolTip,
                      const std::function<void(int)>& setter,
                      const QObject* receiver, Signal signal, QWidget* parent)
{
  auto* layout = new QHBoxLayout();
  auto* widget = new QWidget(parent);
  widget->setLayout(layout);

  auto* labelWidget = new QLabel(label, widget);
  auto* slider      = new QSlider(Qt::Horizontal, widget);
  slider->setRange(min, max);
  slider->setValue(value);
  slider->setToolTip(toolTip);
  auto* valueLabel = new QLabel(QString::number(value), widget);

  QObject::connect(slider, &QSlider::valueChanged, setter);
  QObject::connect(
      static_cast<const typename QtPrivate::FunctionPointer<Signal>::Object*>(
          receiver),
      signal, slider, &QSlider::setValue);
  QObject::connect(slider, &QSlider::valueChanged, valueLabel,
                   static_cast<void (QLabel::*)(int)>(&QLabel::setNum));

  layout->addWidget(labelWidget);
  layout->addWidget(slider);
  layout->addWidget(valueLabel);

  return widget;
}

template <typename Signal>
QWidget* createComboBox(const QString& label, const QString& currentText,
                        const QStringList& items, const QString& toolTip,
                        const std::function<void(const QString&)>& setter,
                        const QObject* receiver, Signal signal, QWidget* parent)
{
  auto* layout = new QHBoxLayout();
  auto* widget = new QWidget(parent);
  widget->setLayout(layout);

  auto* labelWidget = new QLabel(label, widget);
  auto* comboBox    = new QComboBox(widget);
  comboBox->addItems(items);
  comboBox->setCurrentText(currentText);
  comboBox->setToolTip(toolTip);

  QObject::connect(comboBox, &QComboBox::currentTextChanged, setter);
  QObject::connect(
      static_cast<const typename QtPrivate::FunctionPointer<Signal>::Object*>(
          receiver),
      signal, comboBox, &QComboBox::setCurrentText);

  layout->addWidget(labelWidget);
  layout->addWidget(comboBox);

  return widget;
}

template <typename Signal>
QWidget*
createDoubleSpinBox(const QString& label, double value, double min, double max,
                    double singleStep, const QString& toolTip,
                    const std::function<void(double)>& setter,
                    const QObject* receiver, Signal signal, QWidget* parent)
{
  auto* layout = new QHBoxLayout();
  auto* widget = new QWidget(parent);
  widget->setLayout(layout);

  auto* labelWidget = new QLabel(label, widget);
  auto* spinBox     = new QDoubleSpinBox(widget);
  spinBox->setRange(min, max);
  spinBox->setSingleStep(singleStep);
  spinBox->setValue(value);
  spinBox->setToolTip(toolTip);

  QObject::connect(spinBox,
                   static_cast<void (QDoubleSpinBox::*)(double)>(
                       &QDoubleSpinBox::valueChanged),
                   setter);
  QObject::connect(
      static_cast<const typename QtPrivate::FunctionPointer<Signal>::Object*>(
          receiver),
      signal, spinBox, &QDoubleSpinBox::setValue);

  layout->addWidget(labelWidget);
  layout->addWidget(spinBox);

  return widget;
}

template <typename Signal>
QWidget* createSpinBox(const QString& label, int value, int min, int max,
                       const QString&                  toolTip,
                       const std::function<void(int)>& setter,
                       const QObject* receiver, Signal signal, QWidget* parent)
{
  auto* layout = new QHBoxLayout();
  auto* widget = new QWidget(parent);
  widget->setLayout(layout);

  auto* labelWidget = new QLabel(label, widget);
  auto* spinBox     = new QSpinBox(widget);
  spinBox->setRange(min, max);
  spinBox->setValue(value);
  spinBox->setToolTip(toolTip);

  QObject::connect(spinBox,
                   static_cast<void (QSpinBox::*)(int)>(
                       &QSpinBox::valueChanged),
                   setter);
  QObject::connect(
      static_cast<const typename QtPrivate::FunctionPointer<Signal>::Object*>(
          receiver),
      signal, spinBox, &QSpinBox::setValue);

  layout->addWidget(labelWidget);
  layout->addWidget(spinBox);

  return widget;
}

template <typename Signal>
QWidget* createLineEdit(const QString& label, const QString& text,
                        const QString&                             toolTip,
                        const std::function<void(const QString&)>& setter,
                        const QObject* receiver, Signal signal, QWidget* parent)
{
  auto* layout = new QHBoxLayout();
  auto* widget = new QWidget(parent);
  widget->setLayout(layout);

  auto* labelWidget = new QLabel(label, widget);
  auto* lineEdit    = new QLineEdit(text, widget);
  lineEdit->setToolTip(toolTip);

  QObject::connect(lineEdit, &QLineEdit::textChanged, setter);
  QObject::connect(
      static_cast<const typename QtPrivate::FunctionPointer<Signal>::Object*>(
          receiver),
      signal, lineEdit, &QLineEdit::setText);

  layout->addWidget(labelWidget);
  layout->addWidget(lineEdit);

  return widget;
}

} // namespace ControlFactory

#endif // MPVDECK_CONTROLFACTORY_H
