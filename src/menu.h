#ifndef JUCI_MENU_H_
#define JUCI_MENU_H_

#include <string>
#include <unordered_map>
#include <gtkmm.h>

class Menu {
private:
  Menu();
public:
  static Menu &get() {
    static Menu singleton;
    return singleton;
  }
  
  void add_action(const std::string &name, std::function<void()> action);
  std::unordered_map<std::string, Glib::RefPtr<Gio::SimpleAction> > actions;
  void set_keys();
  
  void build();
  
  Glib::RefPtr<Gio::Menu> juci_menu;
  Glib::RefPtr<Gio::Menu> window_menu;

  std::unique_ptr<Gtk::Menu> right_click_line_menu;
  std::unique_ptr<Gtk::Menu> right_click_selected_menu;
  Glib::RefPtr<Gio::Menu> plugin_menu;
  
private:
  Glib::RefPtr<Gtk::Builder> builder;
};
#endif  // JUCI_MENU_H_
