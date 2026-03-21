#include <iostream>
#include <gtkmm/button.h>
#include <gtkmm.h>
#include <gtkmm/application.h>


class MyWindow : public Gtk::Window
{
    public:
        MyWindow(){
            loadCss();
            m_button_not_effective = Gtk::Button("Not Effective");
            m_button_not_effective.signal_clicked().connect(sigc::mem_fun(*this,&MyWindow::onNotEffectiveButtonClicked));
            m_button_super_effective = Gtk::Button("Super Effective");
            m_button_super_effective.signal_clicked().connect(sigc::mem_fun(*this,&MyWindow::onSuperEffectiveButtonClicked));

            {
                m_label_left = Gtk::Label("Left");
                m_label_left.get_style_context()->add_provider(css_provider,GTK_STYLE_PROVIDER_PRIORITY_USER+1);
                m_label_left.add_css_class("label");
                m_label_left.add_css_class("left");
            }

            {
                m_label_right = Gtk::Label("Right");
                m_label_right.get_style_context()->add_provider(css_provider,GTK_STYLE_PROVIDER_PRIORITY_USER+1);
                m_label_right.add_css_class("label");
                m_label_right.add_css_class("right");
            }


            m_layout_grid = Gtk::Grid();
            m_layout_grid.get_style_context()->add_provider(css_provider,GTK_STYLE_PROVIDER_PRIORITY_USER+1);
            for (int x = 0; x < 7; x++)
            {
                for (int y = 0; y < 5; y++)
                {
                    auto empty_label = Gtk::Label("");
                    m_layout_grid.attach(empty_label, x,y);
                }
            }
            
            m_layout_grid.attach(m_label_left, 1, 1, 2);
            m_layout_grid.attach(m_label_right, 4, 1, 2);
            m_layout_grid.set_row_homogeneous(true);
            m_layout_grid.set_column_homogeneous(true);
            m_layout_grid.attach(m_button_not_effective, 2, 3);
            m_layout_grid.attach(m_button_super_effective, 4, 3);
            m_layout_grid.add_css_class("grid");

            set_child(m_layout_grid);
            set_size_request(800,600);
        }
        void onNotEffectiveButtonClicked(){
            std::cout<<"Not effective\n";
        }
        void onSuperEffectiveButtonClicked(){
            std::cout<<"Super effective\n";
        }
        void loadCss(){
            css_provider = Gtk::CssProvider::create();
            css_provider->load_from_path("main.css");
        }
        Gtk::Button m_button_not_effective;
        Gtk::Button m_button_super_effective;
        Gtk::Label m_label_left;
        Gtk::Label m_label_right;
        Gtk::Grid m_layout_grid;
        Glib::RefPtr<Gtk::CssProvider> css_provider;
};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create("org.gtkmm.example");
    return app->make_window_and_run<MyWindow>(argc, argv);
}
