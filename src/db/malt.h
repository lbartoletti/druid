#pragma once

#include <QString>

class DruidMalt {
 public:
  DruidMalt() = default;
  DruidMalt(const QString &name, int ebc, int weight);

  QString name() const;
  void set_name(const QString &name);

  int ebc() const;
  void set_ebc(int ebc);

  int weight() const;
  void set_weight(int weight);

  bool is_valid() const;

 private:
  QString _name;
  int _ebc = 0;
  int _weight = 0;
};
