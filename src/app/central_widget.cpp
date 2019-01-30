#include <iostream>

#include <central_widget.h>
#include "db_selector.h"

DruidCentralWidget::DruidCentralWidget(const DruidDb* const db) : _db(db) {
  setupUi(this);

  _top_layout->insertWidget(0, &_menu);

  _left_layout->addWidget(&_mashing);
  _left_layout->addWidget(&_boiling);
  _left_layout->addWidget(&_fermentation);

  _right_layout->addWidget(&_timer);
  _right_layout->addWidget(&_volume);
  _right_layout->addWidget(&_notes);

  connect(&_menu, &DruidMenu::save, this, &DruidCentralWidget::save);
  connect(&_menu, &DruidMenu::load, this, &DruidCentralWidget::load);
}

void DruidCentralWidget::load() {
  DruidDbSelector selector(_db);
  selector.exec();

  const DruidRecipe recipe = selector.recipe();

  if (recipe.is_valid()) {
    _title->setText(recipe.title());
    _notes.setText(recipe.notes());
    _mashing.set_malts(recipe.malts());
  }
}

void DruidCentralWidget::save() {
  const DruidRecipe recipe = currentRecipe();

  if (!_db->recipe_exists(recipe.title())) {
    _db->recipe_add(recipe);
  } else {
    _db->recipe_update(recipe);
  }
}

DruidRecipe DruidCentralWidget::currentRecipe() const {
  DruidRecipe recipe;

  recipe.set_title(_title->text());
  recipe.set_notes(_notes.text());

  for (const auto malt : _mashing.malts()) {
    recipe.add_malt(malt);
  }

  return recipe;
}
