#include <pebble.h>

//first define all the constanstants
#define NUM_ITEMS_FIRST 4; //the number of menu items in the first section (Breakfast,Lunch,Dinner,Snacks)
#define NUM_SECTIONS 1; //the amount of sections in the menu
#define NUM_ICONS 0 ; //the number of icons in the menu

//the s_ before the variables shows they are static
static Window *s_main_window; // the window for the menu
static Window *s_breakfast_window; // the window for the menu
static Window *s_lunch_window; // the window for the menu
static Window *s_dinner_window; // the window for the menu
static Window *s_snacks_window; // the window for the menu

static MenuLayer *s_menu_layer; //the main menu for the app
static MenuLayer *s_breakfast_layer;
static MenuLayer *s_lunch_layer;
static MenuLayer *s_dinner_layer;
static MenuLayer *s_snacks_layer;


//multidimensional array with the menu items
static char* menu_items[] = {"Breakfast","Lunch","Dinner","Snack"};
static char* breakfast[] = {"Scrambled Eggs", "Egg Sandwich", "Cereal", "Oatmeal"};
static char* lunch[] = {"PB & Jelly","Grilled Cheese","Soup","Ramen"};
static char* dinner[] = {"Chicken Parm","Spaghetti","Ramen","Pizza"};  
static char* snacks[]= {"Parfait","Cookies","Muffins","Cereal"};
static char* data[] = {"Step one:"};

int current_menu=0; //the menu of the current array to loop through
int lengths[] = {4,4,4,4,4};//{4,4,2,2,1}; //the number of items in each menu
//create the callback methods to get information about the menu so we can render it
static int s_current_icon = 0;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
  return NUM_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  switch(section_index){
    case 0:
      return lengths[current_menu];
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Meal");
      break;
  }
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
  //render a section to draw insude
  switch(cell_index->section){
    case 0:
    //then figure out the row we're going to draw
    switch(cell_index->row){
      case 0:
      //Breakfast
        menu_cell_basic_draw(ctx,cell_layer,menu_items[0],"",NULL);
        break;
      case 1:
      //Lunch
        menu_cell_basic_draw(ctx,cell_layer,menu_items[1],"",NULL);
        break;
      case 2:
      //Dinner
        menu_cell_basic_draw(ctx,cell_layer,menu_items[2],"",NULL);
        break;
      case 3:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,menu_items[3],"",NULL);
        break;
    }
    break;
  }
}

static void breakfast_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
    //render a section to draw insude
  switch(cell_index->section){
    case 0:
    //then figure out the row we're going to draw
    switch(cell_index->row){
      case 0:
      //Breakfast
        menu_cell_basic_draw(ctx,cell_layer,breakfast[0],"",NULL);
        break;
      case 1:
      //Lunch
        menu_cell_basic_draw(ctx,cell_layer,breakfast[1],"",NULL);
        break;
      case 2:
      //Dinner
        menu_cell_basic_draw(ctx,cell_layer,breakfast[2],"",NULL);
        break;
      case 3:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,breakfast[3],"",NULL);
        break;
    }
    break;
  }
}

static void lunch_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
    //render a section to draw insude
  switch(cell_index->section){
    case 0:
    //then figure out the row we're going to draw
    switch(cell_index->row){
      case 0:
      //Breakfast
        menu_cell_basic_draw(ctx,cell_layer,lunch[0],"",NULL);
        break;
      case 1:
      //Lunch
        menu_cell_basic_draw(ctx,cell_layer,lunch[1],"",NULL);
        break;
      case 2:
      //Dinner
        menu_cell_basic_draw(ctx,cell_layer,lunch[2],"",NULL);
        break;
      case 3:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,lunch[3],"",NULL);
        break;
    }
    break;
  }
}

  
  static void dinner_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
    //render a section to draw insude
  switch(cell_index->section){
    case 0:
    //then figure out the row we're going to draw
    switch(cell_index->row){
      case 0:
      //Breakfast
        menu_cell_basic_draw(ctx,cell_layer,dinner[0],"",NULL);
        break;
      case 1:
      //Lunch
        menu_cell_basic_draw(ctx,cell_layer,dinner[1],"",NULL);
        break;
      case 2:
      //Dinner
        menu_cell_basic_draw(ctx,cell_layer,dinner[2],"",NULL);
        break;
      case 3:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,dinner[3],"",NULL);
        break;
    }
    break;
  }
  }
static void snacks_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
    //render a section to draw insude
  switch(cell_index->section){
    case 0:
    //then figure out the row we're going to draw
    switch(cell_index->row){
      case 0:
      //Breakfast
        menu_cell_basic_draw(ctx,cell_layer,snacks[0],"",NULL);
        break;
      case 1:
      //Lunch
        menu_cell_basic_draw(ctx,cell_layer,snacks[1],"",NULL);
        break;
      case 2:
      //Dinner
        menu_cell_basic_draw(ctx,cell_layer,snacks[2],"",NULL);
        break;
      case 3:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,snacks[3],"",NULL);
        break;
    }
    break;
  }
}
//method to handle selecting items in the menu
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index,void *data){

  //find out which row (item) is currently selected
  switch(cell_index->row){
    //Breakfast
    case 0:    
       window_stack_push(s_breakfast_window, true);
    break;
    
    //Lunch
    case 1:
     window_stack_push(s_lunch_window, true);
    break;
    
    //Dinner
    case 2:
     window_stack_push(s_dinner_window, true);
    break;
    
    //Snacks
    case 3:
    window_stack_push(s_snacks_window, true);
    break;
  }
}

//load the window
static void main_window_load(Window *window){
  //load any icons
  
  //prepare the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_size = layer_get_frame(window_layer);
  
  //create the menu layer
  s_menu_layer = menu_layer_create(window_size);
    menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  
  
  //make the menu clickable
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  
  
  //add it to the window
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
  
}

//breakfast menu window
static void breakfast_window_load(Window *window){
    //load any icons
  
  //prepare the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_size = layer_get_frame(window_layer);
  
    //create the breakfast layer
  s_breakfast_layer = menu_layer_create(window_size);
    menu_layer_set_callbacks(s_breakfast_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = breakfast_draw_row_callback,
    .select_click = menu_select_callback,
  });

    
  //make the menu clickable
  menu_layer_set_click_config_onto_window(s_breakfast_layer, window);
  
  
  //add it to the window
    layer_add_child(window_layer, menu_layer_get_layer(s_breakfast_layer));
}


//lunch menu window
static void lunch_window_load(Window *window){
    //load any icons
  
  //prepare the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_size = layer_get_frame(window_layer);
  
    //create the breakfast layer
  s_lunch_layer = menu_layer_create(window_size);
    menu_layer_set_callbacks(s_lunch_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = lunch_draw_row_callback,
    .select_click = menu_select_callback,
  });

    
  //make the menu clickable
  menu_layer_set_click_config_onto_window(s_lunch_layer, window);
  
  
  //add it to the window
    layer_add_child(window_layer, menu_layer_get_layer(s_lunch_layer));
}

//dinner menu window
static void dinner_window_load(Window *window){
    //load any icons
  
  //prepare the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_size = layer_get_frame(window_layer);
  
    //create the breakfast layer
  s_dinner_layer = menu_layer_create(window_size);
    menu_layer_set_callbacks(s_dinner_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = dinner_draw_row_callback,
    .select_click = menu_select_callback,
  });

    
  //make the menu clickable
  menu_layer_set_click_config_onto_window(s_dinner_layer, window);
  
  
  //add it to the window
    layer_add_child(window_layer, menu_layer_get_layer(s_dinner_layer));
}

//snacks menu window
static void snacks_window_load(Window *window){
    //load any icons
  
  //prepare the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_size = layer_get_frame(window_layer);
  
    //create the breakfast layer
  s_snacks_layer = menu_layer_create(window_size);
    menu_layer_set_callbacks(s_snacks_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = snacks_draw_row_callback,
    .select_click = menu_select_callback,
  });

    
  //make the menu clickable
  menu_layer_set_click_config_onto_window(s_snacks_layer, window);
  
  
  //add it to the window
    layer_add_child(window_layer, menu_layer_get_layer(s_snacks_layer));
}

//unload method to free up resources
static void main_window_unload(Window *window){
  menu_layer_destroy(s_menu_layer);
  
  //loop through and destory icons if they exist
}

//unload method to free up resources
static void breakfast_window_unload(Window *window){
  menu_layer_destroy(s_breakfast_layer);
  
  //loop through and destory icons if they exist
}
//unload method to free up resources
static void lunch_window_unload(Window *window){
  menu_layer_destroy(s_lunch_layer);
  
  //loop through and destory icons if they exist
}

//unload method to free up resources
static void dinner_window_unload(Window *window){
  menu_layer_destroy(s_dinner_layer);
  
  //loop through and destory icons if they exist
}

//unload method to free up resources
static void snacks_window_unload(Window *window){
  menu_layer_destroy(s_snacks_layer);
  
  //loop through and destory icons if they exist
}

//init method to start everything
static void init(){
  
      s_breakfast_window = window_create();
  
   window_set_window_handlers(s_breakfast_window, (WindowHandlers) {
    .load = breakfast_window_load,
    .unload = breakfast_window_unload,
  });
  
  
      s_lunch_window = window_create();
  
   window_set_window_handlers(s_lunch_window, (WindowHandlers) {
    .load = lunch_window_load,
    .unload = lunch_window_unload,
  });
  
   s_dinner_window = window_create();
  
   window_set_window_handlers(s_dinner_window, (WindowHandlers) {
    .load = dinner_window_load,
    .unload = dinner_window_unload,
  });
  
   s_snacks_window = window_create();
  
   window_set_window_handlers(s_snacks_window, (WindowHandlers) {
    .load = snacks_window_load,
    .unload = snacks_window_unload,
  });
  
  s_main_window = window_create();
  
   window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  

  

  window_stack_push(s_main_window, true);
}

//deinit to free up window resources
static void deinit(){
window_destroy(s_main_window);
  window_destroy(s_breakfast_window);
  window_destroy(s_lunch_window);
  window_destroy(s_dinner_window);
  window_destroy(s_snacks_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
