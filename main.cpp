/* copyright (c) Grant Rostig 2019, all rights reserved
 *
 * Generated as part of Grant's password_manager, file_maintenance effort to show
 * how the Overloaded pattern is used by std::visit.
 *
 * This example code shows how overloaded could be applied to a menu system.
 * The menu system allows for various function signatures
 * and various configurations of return values.
 */
#include "global_entities.h"
#include "state_menu.h"
#include "state_application.h"
#include "menu.h"
#include "process_menu.h"
#include "interaction_result.h"
#include "field_navigation_interaction_map.h"
//#include <bits/stdc++.h>
#include <iostream>
#include <memory>
#include <functional>
#include <variant>
#include <cassert>
#include <csignal>
#include <iostream>
#include <stacktrace>
#include <source_location>

using std::cin; using std::cout; using std::cerr; using std::clog; using std::endl; using std::string;  // using namespace std;
using namespace std::string_literals;

/* few items for debugging purposes */
/// define NDEBUG if asserts are NOT to be checked.  Put in *.h file not *.CPP
//#define 	NDEBUG
/// define GR_DEBUG if we/programmer is Debugging.  Put in *.h file not *.CPP
//#define  	GR_DEBUG
//#ifdef   	GR_DEBUG
//#endif  # GR_DEBUG
/// Some crude logging that privides source location.
//#define LOGGER_( msg )   using loc = std::source_location;std::cerr<<"\n\r["<<loc::current().file_name()<<':'<<std::setw(3)<<loc::current().line()<<','<<std::setw(2)<<loc::current().column()<<"]`"<<loc::current().function_name()<<"`:{" <<#msg<<"}.\n";
//#define LOGGERS( msg, x )using loc = std::source_location;std::cerr<<"\n\r["<<loc::current().file_name()<<':'<<std::setw(3)<<loc::current().line()<<','<<std::setw(2)<<loc::current().column()<<"]`"<<loc::current().function_name()<<"`:{" <<#msg<<"},{"<<x<<"}.\n";
/// To turn off debugging just truncate the macro
//#define LOGGER_( msg )
//#define LOGGERS( msg,x )

/// gives a source location for printing.  Used for debugging.
std::string source_loc();
/// gives a source location for printing.  Used for debugging.
void crash_tracer(int const signal_number);
void crash_signals_register();

int main ( int argc, char* arv[] ) { string my_arv { *arv}; cout << "~~~ argc,argv:"<<argc<<","<<my_arv<<"."<<endl;
    cin.exceptions( std::istream::failbit);  // throw on fail of cin.
    crash_signals_register();
    LOGGER_ ("********************************BEGIN PROGRAM**************************************");
#ifndef NDEBUG
    fieldNavInteractionIntentTable.verify();  // TODO: only needed during debugging and testing.
#endif

    Menu menu_main; // create main first, then the sub-menus.
    Menu menu_edit;
    Menu menu_file;
    Menu menu_settings;
    State_menu state {};  /// instead of global variables, we pass this around instead.
    // link? menus together starting at the main menu. TODO: show example of a sub-sub menu.
    state.setMenu_main( std::make_shared<Menu>(menu_main) ); // record menu pointers in state
    state.setMenu_edit( std::make_shared<Menu>(menu_edit) );
    state.setMenu_file( std::make_shared<Menu>(menu_file) );
    state.setMenu_settings( std::make_shared<Menu>(menu_settings) );

    State_application application_state {};
    state.setApplication_data_sp( std::make_shared<State_application>(application_state) );

    // Create the menu options
    initialize_menu_main_options( state, state.getMenu_main());
    initialize_menu_edit_options( state, state.getMenu_edit());
    initialize_menu_file_options( state, state.getMenu_file());
    initialize_menu_settings_options( state, state.getMenu_settings());

    InteractionResult ir = process_main_menu( state );
    LOGGERS("ir.hot_key_row.my_name:", ir.to_string());

    /* Debugging/dev history:
    // IO_table io_table_persons;
    // auto r = action_io_row_create( state, io_table_persons );

    // cout << "Table: " << io_table_persons.spec.name<<endl;
    // cout << "name: " << io_row_person.name << endl;
    // io_row_person.print();
    // io_table_persons.print_rows();

//    using Data_type_variant = std::variant<
//        Data_type_boolean,
//        Data_type_character,
//        Data_type_uint64,
//        Data_type_time_point,
//        Data_type_tm,
//        Data_type_alphanumeric,
//        Data_type_integer,
//        Data_type_decimal,
//        Data_type_scientific

//	  struct InteractionResult {
//		  InteractionResultData_Variant  data 		{};
//		  InteractionResultError		   error 		{};
//		  InteractionResultNav 	       navigation 	{InteractionResultNav::na};

    // auto [data, error, navigation ] = action_dialog_modal_io_field( state, io_table_persons.spec.fields.at(2) );
    // std::optional<ValidationFieldError> error_opt = std::get<IO_field_spec_integer>(io_table_persons.spec.fields.at(2)).validate_data( value_plus );

// debugging of ref and * setting of location
     std::string target_data_value0 	= std::get<IO_row_person *>( io_table_persons.spec.io_candidate_row )->name;
    cerr << "main():  target_data_value: "<<target_data_value0<<endl;
    cerr << "main():  target_data_location: "<< &( (std::get<IO_row_person *>( io_table_persons.spec.io_candidate_row ))->name ) <<endl;
    (std::get<IO_field_spec_alphanumeric>(io_table_persons.spec.fields.at(0))) .setData_value( "name**from**main." );

    int target_data_value2 			= std::get<IO_row_person *>( io_table_persons.spec.io_candidate_row )->age;
    cerr << "main():  target_data_value: "<<target_data_value2<<endl;
    int * target_data_location = &( (std::get<IO_row_person *>( io_table_persons.spec.io_candidate_row ))->age );
    cerr << "main():  target_data_location: "<<target_data_location<<endl;
    // (std::get<IO_field_spec_integer     >(io_table_persons.spec.fields.at(2))) .setData_location( target_data_location );

    integer     >(io_table_persons.spec.fields.at(2)))
            .setData_location(
                &( (std::get<IO_row_person *>( io_table_persons.spec.io_candidate_row ))->age ));

    bool target_data_value3 = std::get<IO_row_person *>( io_table_persons.spec.io_candidate_row )->is_cpp_programmer;
    cerr << "main():  is_cpp target_data_value: "<<target_data_value3<<endl;
    cerr << "main():  is_cpp target_data_location: "<< &( (std::get<IO_row_person *>( io_table_persons.spec.io_candidate_row ))->is_cpp_programmer ) <<endl;
    // (std::get<IO_field_spec_alphanumeric>(io_table_persons.spec.fields.at(0))) .setData_location( &( (std::get<IO_row_person *>( io_table_persons.spec.temp_row ))->name );

    //io_table_persons.print();
    //auto r = action_io_row_create( state, io_table_persons );
    //io_table_persons.print();
    //auto r2 = action_io_row_create( state, io_table_persons );
    //io_table_persons.print();

    //std::shared_ptr< std::optional< Data_type_integer >> location = std::get< IO_field_spec_integer> ( io_table_persons.spec.fields.at(2) ).data_location_opt ;
    //    *location = std::optional< Data_type_integer > {66};

    cout << endl;
    cout << "data_value    :" <<  std::get_if< IO_field_spec_integer >( &io_table_persons.spec.fields.at(2) )->data_value     << endl;
    cout << "data_value_opt:" << *std::get_if< IO_field_spec_integer >( &io_table_persons.spec.fields.at(2) )->data_value_opt	 <<endl ;

    cout << "temp_row_age:"   << *std::get_if< IO_row_person >( &io_table_persons.spec.temp_row )->age << endl;


    cout <<  *std::get<IO_row_person >( io_table_persons.rows.at(0) ).name 				<< ", ";
    cout <<  *std::get<IO_row_person >( io_table_persons.rows.at(0) ).balance 			<< ", ";

    complexity introduced for std::optional demonstration  note: data structure has changed, but the pattern is still valid
    cout << *std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age 					<< " (could throw), ";  // de-reference the optional
    cout <<  std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age.value() 			<< " (could throw), ";  // de-reference the optional
    cout << (std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age ? "bool: present" : "bool: not present")		<< ", ";  // de-reference the optional
    cout << (std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age.has_value() ? "has_value: present" : "has_value: not present")		<< ", ";  // de-reference the optional
    cout << (std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age.has_value() ? *std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age : -99 )		<< ", ";  // de-reference the optional
    cout <<  std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age.value_or(-99)	<< " (alt value must be same type), ";  // de-reference the optional

    auto picker = [=] () -> std::variant<int, std::string> {
        if (std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age.has_value()) {
             cout << "\npicker(): " << *std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age << endl;
             return *std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age;
             // int temp =  *std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age;
             // return *std::get< std::vector<IO_row_person> >( io_table_persons.rows ).at(0).age;
             // return temp;
        }
        else return "NULL";
    };
    std::variant<int, std::string> solid_picker_variant = picker();  // TODO: TODO: how can I solidify a temp variable for the use in 'cout', other than creating a variable?
    cout << "\nint version: " << *std::get_if<int> (&solid_picker_variant)<< endl;
    cout << " OR get_if  string version: " <<  std::get_if<std::string> (&solid_picker_variant) << endl;
    cout << " OR complex string version: " << ( std::get_if<std::string> (&solid_picker_variant) ? *std::get_if<std::string>(&solid_picker_variant) : "") << endl;
    cout << "." << endl;
    // end of std::optional complexity demonstration
    */

    LOGGER_("\n********************************END!! OF PROGRAM**************************************");
    return 0;
}
