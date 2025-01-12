#include "lib_tty.hpp"
#include "interaction_result.h"
#include "global_entities.h"
#include "state_menu.h"
#include "menu.h"
#include "menu_option.h"
#include "process_menu.h"
#include "menu_actions.h"
#include "window_panel.h"
#include <iostream>
#include <cassert>

/// Loop until we return a menu_selection selected by the user from the menu_options that were prompted.
/// TODO: allow arrow keys to select? or allow a number representing the entry to be selected??
Menu_option const &
input_menu_selection( State_menu & state_menu, std::shared_ptr<Menu> const menu_sp ) {
    assert( menu_sp     && "Precondition." );  // TODO??: is this the correct check?
    //assert( state_menu  && "Precondition." );  // TODO??: is this the correct check?
    while (true) {
        auto r = pagination( state_menu, {.height= 1, .width= menu_sp->name.length() + 3 + 1} );  // +1 for cursor position. // TODO: complete this above.
        cout << menu_sp->name<<">> "; cout.flush();
        pagination_reset( state_menu, {0,0});
        auto const [key_char_i18ns, hot_key_row, file_status] { Lib_tty::get_kb_keystrokes_raw( 1, false, true, true )};
        // *** handle file_status *** // TODO: this
        // *** handle Hot_key_rows *** // WARNING NOTE: TODO: improve this! Here we link the hot_key_row to the regular key for getting "help", ie. F1 key. The mapping is found in lib_tty::consider_Hot_key_row()::Hot_key_rows.
        Lib_tty::Key_chars_i18n user_menu_selection { (hot_key_row.function_cat == Lib_tty::HotKeyFunctionCat::help_popup || (key_char_i18ns.at(0) == '?')) ? Lib_tty::Key_chars_i18n {'h'} : key_char_i18ns };  // TODO: h and ? are magic numbers.
        // *** handle actual selection ***
        for ( Menu_option const & menu_option: menu_sp->options ) {
            assert( user_menu_selection.size() == 1 && "Logic error: currenlty only single char menu selections are allowed.");
            if ( user_menu_selection.at(0) == menu_option.input_token[0] ) { // TODO: only allows for length 1 one menu selections as shown 5 lines up.
                cout << endl;           // We finish our prompt and user input sucessfully.
                assert( menu_option.name != STRING_NULL && "Precondition." );
                return  menu_option;     //RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
            }
        }
        cout << "\aInvalid menu selection or key press, try again, or press the single <h> key for (h)elp." << endl;
    }
    assert(false && "We should never get here.");  // infinute loop that is exited via a menu selection.  Should never get here, obviously not needed, but placed here for clarity. TODO:  fix this up.
}

// The 'Overloaded' pattern _could_ have been standardized in c++20 but wasn't, as per Stroustup in A Tour of C++ 2nd Edition, Page 176.
template<class... Ts>  				// variadic ie. any number of any type are handled. Below we use several in the contructor of our one object. // ... is a parameter pack.
struct Overloaded : Ts... { 		// inherit from all those types  TODO:  how does this work??
    using Ts::operator()...;  		// all of those operator()s.  // new in c++17.
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;  // deduction guide.  -> = returns.  TODO: explain: what returns what to whom??

InteractionResult const process_menu_selection( State_menu & state, Menu_option const & menu_selection ) {  /// run the menu action
    // get some general data, even though it is not used for every overload.
    std::shared_ptr<Menu>   current_menu_sp { state.menu_top_sp() };  	// TODO: I don't think I need a value in the lambda, because it is overridden at std::visit?
    Menu 		    		current_menu 	{ *current_menu_sp };
    State_menu		    	kludge_state	{};
    /* Some prior development ideas preserved for understanding of the history
    //Menu 						kludge_menu			{};
    // std::shared_ptr<Menu> 	kludge_menu_sp 		{};
    // IO_table  				kludge_table   		{}; 			// state.io_table_sp();  // TODO: I don't think I need a value in the lambda, because it is overridden at std::visit?A
    // IO_row_variant 			kludge_row_var 		{};
    // Row_range				kludge_row_range 	{};
    // size_t					kludge_row_index 	{};
    // std::filesystem::path	kludge_file_path 	{};
    // if ( std::get<>(menu_selection.menu_action_fn) != nullptr) {
    // auto r = std::get<std::function<Action_post_return_struct(State&)>>(menu_selection.menu_action_fn_variant)(state);
    */

    auto function_object_overload_set = Overloaded {
        [&kludge_state ]  			   ( std::function< InteractionResult(State_menu &              ) > &af)
            { auto r = af( kludge_state );
              //LOGGER_(" ");LOGGER_( "[] state overload.");
              return r;
            }, 						// to be invoked via operator() on the function object (AKA lambda) or any callable object.
        [&kludge_state, &current_menu] ( std::function< InteractionResult(State_menu &, Menu const &) > &af)
            { auto r = af( kludge_state, current_menu );
              LOGGER_(" ");LOGGER_("[] state,menu overload.");
              return r;
            }, 						// to be invoked via operator() on the function object (AKA lambda) or any callable object.
        /* Some prior development ideas preserved for understanding of the history
        //[]    							(std::function<InteractionResult()> 									        &af) { auto r = af(); cerr<<"\nvoid overload.\n";        			    return r; }, // to be invoked via operator() on the function object (AKA lambda) or any callable object.
        //[&kludge_state, &kludge_menu_sp]	(std::function<InteractionResult(State_menu &, std::shared_ptr<Menu>)> 	&af) { auto r = af( kludge_state, kludge_menu_sp ); cerr<<"\nmenu_sp overload.\n";    			    return r; }, // to be invoked via operator() on the function object (AKA lambda) or any callable object.
        //[&kludge_state, &kludge_table]	(std::function<InteractionResult(State_menu &, IO_table &)> 			&af) { auto r = af( kludge_state, kludge_table );cerr<<"\ntable overload.\n";    			    return r; }, // to be invoked via operator() on the function object (AKA lambda) or any callable object.
        //[&kludge_state, &kludge_table]	(std::function<InteractionResult(State_menu &, IO_table const &)> 			&af) { auto r = af( kludge_state, kludge_table );cerr<<"\ntable overload.\n";    			    return r; }, // to be invoked via operator() on the function object (AKA lambda) or any callable object.
        //[&state ]  						(std::function<decltype(action_program_exit_with_prompts)> &af) { auto r = af(state);          			return r; }, // to be invoked via operator() on the function object (AKA lambda) or any callable object.
        //[&state, &menu]    				(std::function<decltype(action_print_menu)> &af) 				{ auto r = af(state,menu);      		return r; }, // NOTE: each of these must also appear in Menu_action_fn_variant type!
        //[&state, &menu_sp] 				(std::function<decltype(process_menu)> &af) 					{ auto r = af(state,menu_sp); 			return r; },
        //[&state, &kludge_table]  			(std::function<decltype(action_io_row_create)> &af)				{ auto r = af(state,kludge_table);    	return r; },
        //[&state, &kludge_row_var] 		(std::function<decltype(action_io_row_print)> &af)				{ auto r = af(state,kludge_row_var);    return r; },
        //[&state, &kludge_table, &kludge_row_index]   (std::function<decltype(action_io_row_print_index)> &af)	{ auto r = af(state,kludge_table,kludge_row_index);    	return r; },
        //[&state, &kludge_table, &kludge_row_range]   (std::function<decltype(action_io_row_list_rows)> &af)	{ auto r = af(state,kludge_table,kludge_row_range);    	return r; },
        //[&state, &kludge_file_path]   	(std::function<decltype(action_save_as_changes_to_disk)> &af)		        { auto r = af(state,kludge_file_path);  return r; }
      */
    };

    auto fo_return_type_overloaded_set = [ & ] (auto... args) -> InteractionResult
                                            { return function_object_overload_set( args...); };  // not needed if all return types are the same, which is the current case and that is used in the return type of this function.
    InteractionResult action_result = std::visit( fo_return_type_overloaded_set, menu_selection.menu_action_fn_variant );
    return action_result;
    // some history: } else  // return menu_selection.option_value;  TODO: this may or may not be applicable depending on the use of menus to select values to be used by other? functions.
}

InteractionResult const
process_menu( State_menu & 			state, 	   		// needed for menu and application, like a global variable/singleton.  TODO: improve this?
              std::shared_ptr<Menu> menu_sp  )  { 	// the menu we will run/process
    assert( menu_sp != nullptr && "We must have a menu to process");
    state.push_menu_sp( menu_sp ); // our currently active menu is stored at top of stack
    while (true) {  /* input menu selection and run it */
        Menu_option const 		menu_option  	{ input_menu_selection(   state, menu_sp )};
        InteractionResult const ir     			{ process_menu_selection( state, menu_option )};
        switch ( ir.navigation ) { 					// post processing: load the correct menu to be shown to user, or exit.
        case Lib_tty::InteractionIntentNav::retain_menu:  	// we did something on this menu and we stay on current menu and just loop.
            break;
        case Lib_tty::InteractionIntentNav::prior_menu:   	// we have been asked to go back one level of menu so we get and set that menu, and loop.
            menu_sp = state.menu_pop_top_sp();
            break;
        case Lib_tty::InteractionIntentNav::main_menu:   	// we have been asked to go directly to main menu so we set that menu and loop.
            menu_sp = state.menu_pop_to_sp(state.getMenu_main());
            break;
        case Lib_tty::InteractionIntentNav::exit_all_menu:   	// we have been asked to go directly to main menu so we set that menu and loop.
            return { {}, {}, {}, {}, Lib_tty::InteractionIntentNav::exit_all_menu };  // returning to main to get out. return result is probably not needed?

        // *** above are valid return values from running an "action_", *** below are simply debugging code.
        case Lib_tty::InteractionIntentNav::exit_pgm_immediately:
            // return { {}, {}, {}, {}, InteractionResultNav::exit_pgm_immediately };  // returning to main to get out. return result is probably not needed?
        case Lib_tty::InteractionIntentNav::exit_pgm_with_prompts:
            // return { {}, {}, {}, {}, InteractionResultNav::exit_pgm_with_prompts };  // returning to main to get out. return result is probably not needed?
        case Lib_tty::InteractionIntentNav::continue_forward_pagination:
        case Lib_tty::InteractionIntentNav::continue_backward_pagination:
        case Lib_tty::InteractionIntentNav::exit_fn_immediately:
        case Lib_tty::InteractionIntentNav::exit_fn_with_prompts:
        case Lib_tty::InteractionIntentNav::prior_menu_discard_value:
        case Lib_tty::InteractionIntentNav::up_one_field:
        case Lib_tty::InteractionIntentNav::down_one_field:
        case Lib_tty::InteractionIntentNav::up_one_block:
        case Lib_tty::InteractionIntentNav::down_one_block:
        case Lib_tty::InteractionIntentNav::save_form_as_is:
        case Lib_tty::InteractionIntentNav::skip_one_field:
        case Lib_tty::InteractionIntentNav::skip_to_end_of_fields:
        case Lib_tty::InteractionIntentNav::next_row:
        case Lib_tty::InteractionIntentNav::prior_row:
        case Lib_tty::InteractionIntentNav::first_row:
        case Lib_tty::InteractionIntentNav::last_row:
        case Lib_tty::InteractionIntentNav::na:
        case Lib_tty::InteractionIntentNav::no_result :  // TODO: correct?
        // TODO assert(( "Logic error: process_menu: invalid InteractionResultNav after doing menu selection.", false));
            break;
        }
    }
}

InteractionResult const process_main_menu(State_menu & state ) {  // cannot overload process_menu, or Overload become ambiguous.  TODO??: Is there a fix/cast?
    action_print_menu_help( state, *state.getMenu_main());
    return process_menu( state, state.getMenu_main());
}
