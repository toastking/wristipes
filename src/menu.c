#include <pebble.h>

//first define all the constanstants
#define NUM_ITEMS_FIRST 4; //the number of menu items in the first section (Breakfast,Lunch,Dinner,Snacks)
#define NUM_SECTIONS 1; //the amount of sections in the menu
#define NUM_ICONS 0 ; //the number of icons in the menu

//the s_ before the variables shows they are static
static Window *s_main_window; // the window for the menu
static MenuLayer *s_menu_layer; //the main menu for the app
//multidimensional array with the menu items
static char* menu_items[] = {"Breakfast","Lunch","Dinner","Snack", "Scrambled Eggs", "Egg Sandwich", "Cereal", "Oatmeal", "PB & Jelly","Grilled Cheese","Soup","Ramen",  "Chicken Parm","Spaghetti","Ramen","Pizza",  "Parfait","Cookies","Muffins","Cereal"};
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
        menu_cell_basic_draw(ctx,cell_layer,menu_items[current_menu+lengths[current_menu]+0],"",NULL);
        break;
      case 1:
      //Lunch
        menu_cell_basic_draw(ctx,cell_layer,menu_items[current_menu+lengths[current_menu]+1],"",NULL);
        break;
      case 2:
      //Dinner
        menu_cell_basic_draw(ctx,cell_layer,menu_items[current_menu+lengths[current_menu]+2],"",NULL);
        break;
      case 3:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,menu_items[current_menu+lengths[current_menu]+3],"",NULL);
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
      current_menu = 1;
    break;
    
    //Lunch
    case 1:
    current_menu = 2;
    break;
    
    //Dinner
    case 2:
    current_menu = 3;
    break;
    
    //Snacks
    case 3:
    current_menu = 4;
    break;
  }
  //reload the data
  menu_layer_reload_data(menu_layer);
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

//unload method to free up resources
static void main_window_unload(Window *window){
  menu_layer_destroy(s_menu_layer);
  
  //loop through and destory icons if they exist
}

//init method to start everything
static void init(){
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
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
