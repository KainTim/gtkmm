#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <gtkmm/button.h>
#include <gtkmm.h>
#include <gtkmm/application.h>
#include "type.cpp"


class MyWindow : public Gtk::Window
{
    public:
        MyWindow(){
            loadTypesFromCsv("chart.csv");

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
            
            {
                m_layout_grid.attach(m_label_left, 1, 1, 2);
                m_layout_grid.attach(m_label_right, 4, 1, 2);
                m_layout_grid.set_row_homogeneous(true);
                m_layout_grid.set_column_homogeneous(true);
                m_layout_grid.attach(m_button_not_effective, 2, 3);
                m_layout_grid.attach(m_button_super_effective, 4, 3);
                m_layout_grid.add_css_class("grid");
            }

            set_child(m_layout_grid);
            set_size_request(800,600);
        }
    private:
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
        void loadTypesFromCsv(std::string path){
            std::ifstream in_file;
            in_file.open(path);
            std::cout << "Is open:" << (in_file.is_open()?"true":"false")<<std::endl;
            std::string line;
            // Get the first line
            std::getline(in_file, line);
            parseFirstCsvLine(line);
            // Parse the Rest
            while(std::getline(in_file, line)) {
                std::cout << line << '\n';
                std::regex outer_delimiter(";");
                std::sregex_token_iterator outer_iterator(line.begin(), line.end(), outer_delimiter, -1);
                std::sregex_token_iterator outer_end;
                std::string type_string = *outer_iterator++;
                Type* currentType;
                for (auto it = m_types.begin(); it != m_types.end(); ++it) {
                    if (it->nameMatches(type_string))
                    {
                        currentType = &*it;
                        std::cout << "found match for "<<type_string << " Id: "<<currentType->id;
                    }
                }
                
                std::string resistant_string = *outer_iterator++;
                auto resistantIds = parseSubCsv(resistant_string);


                std::string weak_string = *outer_iterator;
                auto weakIds = parseSubCsv(weak_string);
            }
        }
        std::vector<int> parseSubCsv(std::string part){
            std::cout << part << '\n';
            std::regex delimiter(",");
            std::sregex_token_iterator regex_iterator(part.begin(), part.end(), delimiter, -1);
            std::sregex_token_iterator regex_end;
            std::vector<int> result = std::vector<int>();
            while (regex_iterator != regex_end) {
                auto type_string = *regex_iterator;
                
                for (auto it = m_types.begin(); it != m_types.end(); ++it) {
                    if (it->nameMatches(type_string))
                    {
                        result.push_back(it->id);
                        std::cout<< it->id<< ", ";
                    }
                }
                ++regex_iterator;
            }
            std::cout<<std::endl;
            return result;
        }

        void parseFirstCsvLine(std::string line){
                std::cout << line << '\n';
                std::regex delimiter(";");
                std::sregex_token_iterator iterator(line.begin(), line.end(), delimiter, -1);
                std::sregex_token_iterator end;
                while (iterator != end) {
                    m_types.push_back(Type(*iterator));
                    ++iterator;
                }
        }
        std::vector<Type> m_types = std::vector<Type>();
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
