#include <iostream>

#include <central_widget.h>

DruidCentralWidget::DruidCentralWidget(const DruidDb* const db) : _db(db) {
  setupUi(this);

  _top_layout->insertWidget(0, &_menu);

  _right_layout->addWidget(&_timer);
  _right_layout->addWidget(&_volume);
  _right_layout->addWidget(&_notes);

  connect(&_menu, &DruidMenu::save, this, &DruidCentralWidget::save);
  connect(&_menu, &DruidMenu::load, this, &DruidCentralWidget::load);
}

void DruidCentralWidget::load() {}

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

  recipe.setTitle(_title->text());
  recipe.setNotes(_notes.text());

  return recipe;
}
