#include "menu.h"
#include "config.h"
#include <string>
#include <iostream>

const Glib::ustring menu_xml= R"RAW(<interface>
  <menu id='right-click-line-menu'>
    <section>
      <item>
        <attribute name='label' translatable='yes'>_Undo</attribute>
        <attribute name='action'>app.edit_undo</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Redo</attribute>
        <attribute name='action'>app.edit_redo</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name='label' translatable='yes'>_Paste</attribute>
        <attribute name='action'>app.edit_paste</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name='label' translatable='yes'>_Toggle _Comments</attribute>
        <attribute name='action'>app.source_comments_toggle</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name='label' translatable='yes'>_Go _to _Declaration</attribute>
        <attribute name='action'>app.source_goto_declaration</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Go _to _Implementation</attribute>
        <attribute name='action'>app.source_goto_implementation</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Go _to _Usage</attribute>
        <attribute name='action'>app.source_goto_usage</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Go _to _Method</attribute>
        <attribute name='action'>app.source_goto_method</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Rename</attribute>
        <attribute name='action'>app.source_rename</attribute>
      </item>
    </section>
  </menu>
  <menu id='right-click-selected-menu'>
    <section>
      <item>
        <attribute name='label' translatable='yes'>_Undo</attribute>
        <attribute name='action'>app.edit_undo</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Redo</attribute>
        <attribute name='action'>app.edit_redo</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name='label' translatable='yes'>_Cut</attribute>
        <attribute name='action'>app.edit_cut</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Copy</attribute>
        <attribute name='action'>app.edit_copy</attribute>
      </item>
      <item>
        <attribute name='label' translatable='yes'>_Paste</attribute>
        <attribute name='action'>app.edit_paste</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name='label' translatable='yes'>_Toggle _Comments</attribute>
        <attribute name='action'>app.source_comments_toggle</attribute>
      </item>
    </section>
  </menu>
)RAW";

void Menu::add_action(const std::string &name, std::function<void()> action) {
  auto g_application=g_application_get_default();
  auto gio_application=Glib::wrap(g_application, true);
  auto application=Glib::RefPtr<Gtk::Application>::cast_static(gio_application);
  
  actions[name]=application->add_action(name, action);
}

void Menu::set_keys() {
  auto g_application=g_application_get_default();
  auto gio_application=Glib::wrap(g_application, true);
  auto application=Glib::RefPtr<Gtk::Application>::cast_static(gio_application);
           
  for(auto &key: Config::get().menu.keys) {
    if(key.second.size()>0 && actions.find(key.first)!=actions.end())
      application->set_accel_for_action("app."+key.first, key.second);
  }
}

void Menu::build() {
  try {
    builder = Gtk::Builder::create_from_string(menu_xml);
    auto object = builder->get_object("juci-menu");
    juci_menu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    object = builder->get_object("window-menu");
    window_menu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    object = builder->get_object("right-click-line-menu");
    auto ptr = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    right_click_line_menu = std::make_unique<Gtk::Menu>(ptr);
    object = builder->get_object("right-click-selected-menu");
    ptr = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
    right_click_selected_menu = std::make_unique<Gtk::Menu>(ptr);
  }
  catch (const Glib::Error &ex) {
    std::cerr << "building menu failed: " << ex.what();
  }
}
