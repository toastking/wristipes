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
static Window *s_convert_window;
static Window *s_recipe_window; //the window to display a recipe

static MenuLayer *s_menu_layer; //the main menu for the app
static MenuLayer *s_breakfast_layer;
static MenuLayer *s_lunch_layer;
static MenuLayer *s_dinner_layer;
static MenuLayer *s_snacks_layer;
static MenuLayer *s_convert_layer; 

static TextLayer *s_cup_layer; //convert layer
static TextLayer *s_oz_layer;
static TextLayer *s_tbp_layer;

//variables for the scrolling recipe display
static TextLayer*s_recipe_layer;
static ScrollLayer *s_scroll_layer;
static InverterLayer *s_invert_layer;

//the icons for the main menu
static GBitmap *menu_icons[4];

//the actual recipe data
static char* breakfast_recipes[] = {"Fried Eggs\nIngredients\n-1 egg\n-oil\n-salt and pepper, to taste\n1. Put a pan on medium heat and put oil on the bottom of the pan.\n2. Crack an egg into the pan.\n3. Wait till the egg whites are fully cooked.\n4. Remove and add salt and pepper to taste.", "Fried Rice\n- 2 cups rice, cooked\n- 2 eggs\n- any sort of pre-cooked things you want in your rice\n- soy sauce\n1. Cook rice and store in fridge overnight to dry.\n2. Oil pan on medium heat.\n3. Dump pre-cooked ingredients into oiled pan and heat up to an appropriate temperature.\n4. Crack two eggs into the pan and beat mixture.\n5. Add rice and mix.\n6. Add soy sauce and mix. Repeat til rice is a nice brown color.\n7. Enjoy."};
static char* dinner_recipes[] = {"Steak  au  Poivre\nIngredients  :  \n-4 sirloin steaks or filet mignons, about 8oz (225g) each  \n-½ tsp dry mustard  \n-12 tsp black peppercorns  \n-2 tbsp vegetable oil  \n-¼ cup sherry or brandy  \n-2/3 cup creme fraiche\n\nDirections  :  \n1. Trim any excess fat from the steaks. If using filet mignons, flatten slightly with a meat mallet. Sprinkle with the mustard.  \n2. Crush the peppercorns in a mortar or under a saucepan and press on both sides of the steaks.  \n3. Heat the oil in a large frying pan over high heat. Add the steaks and cook, 23 minutes each side for a rare steak, 4 minutes each side for medium, and 56 minutes each side for well done. Transfer to a platter and tent with aluminum foil.\n4. Add the sherry to the pan and stir up the browned bits. Add the créme fraîche and cook for 3 minutes, until slightly thickened. Place the steaks on individual plates and top with the sauce. Serve hot.  "};

//multidimensional array with the menu items
static char* menu_items[] = {"Breakfast","Lunch","Dinner","Snack", "Conversions"};
static char* breakfast[] = {"Fried Eggs", "Fried Rice", "Cereal", "Oatmeal"};
static char* lunch[] = {"PB & Jelly","Grilled Cheese","Soup","Ramen"};
static char* dinner[] = {"Chicken Parm","Spaghetti","Ramen","Pizza"};  
static char* snacks[]= {"Parfait","Cookies","Muffins","Cereal"};
static char* data[] = {"Step one:","Step 2:","Step 3:"};
static double convert[] = {0, 0, 0}; //(oz, cup tblspoon)

int current_menu=0; //the menu of the current array to loop through
int lengths[] = {5,4,4,4,4};//{4,4,2,2,1}; //the number of items in each menu
//create the callback methods to get information about the menu so we can render it
static int s_current_index = 0;

static void update_text_layers(){
  char* output= "1234 cups";
  
  //int num1 = (int)(convert[1]%(10.00));
  //convert the output
  snprintf(output,sizeof(output),"%d oz",(int)convert[0]);
  text_layer_set_text(s_oz_layer, output);
  //snprintf(output,sizeof(output),"%d.%d cups",(int)(convert[1]),(int)convert[1]%10.0);
  text_layer_set_text(s_cup_layer, output);
  snprintf(output,sizeof(output),"%a tbsp",convert[2]);
  text_layer_set_text(s_tbp_layer, output);
  
}

//event handlers
static void down_single_click_handler(){
  convert[0] = convert[0]- 1;
  convert[1] = convert[0] * 0.125;
  convert[2] = convert[0] * 2;
  
  update_text_layers();
}

static void up_single_click_handler(){
  convert[0] = convert[0]+ 1;
  convert[1] = convert[0] * 0.125;
  convert[2] = convert[0] * 2;
  
  update_text_layers();
}


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
        menu_cell_basic_draw(ctx,cell_layer,menu_items[0],"",menu_icons[0]);
        break;
      case 1:
      //Lunch
        menu_cell_basic_draw(ctx,cell_layer,menu_items[1],"",menu_icons[1]);
        break;
      case 2:
      //Dinner
        menu_cell_basic_draw(ctx,cell_layer,menu_items[2],"",menu_icons[2]);
        break;
      case 3:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,menu_items[3],"",menu_icons[3]);
        break;
      case 4:
      //Snacks
        menu_cell_basic_draw(ctx,cell_layer,menu_items[4],"",NULL);
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
    
    //convert
    case 4:
    window_stack_push(s_convert_window, true);
    break;
  }
}

static void update_text_layer(Layer* layer,GContext *ctx ){

}

//method to handle selecting items in the menu
static void breakfast_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index,void *data){

  //find out which row (item) is currently selected
  switch(cell_index->row){
    //first recipe
    case 0:    
  text_layer_set_text(s_recipe_layer, breakfast[0]);
    window_stack_push(s_recipe_window, true);
    break;
    
  //second meal
    case 1:
      //Breakfast
  text_layer_set_text(s_recipe_layer, breakfast_recipes[1]);  
    window_stack_push(s_recipe_window, true);
    break;
    
    //Dinner
    case 2:
      //Breakfast
  text_layer_set_text(s_recipe_layer, breakfast_recipes[0]);
    window_stack_push(s_recipe_window, true);
    break;
    
    //Snacks
    case 3:
    window_stack_push(s_snacks_window, true);
    break;
    
    //convert
    case 4:
    window_stack_push(s_convert_window, true);
    break;
  }
}


//load the window
static void main_window_load(Window *window){
  //load any icons
  
  menu_icons[0] = gbitmap_create_with_resource(RESOURCE_ID_BREAKFAST);
    menu_icons[1] = gbitmap_create_with_resource(RESOURCE_ID_LUNCH);
    menu_icons[2] = gbitmap_create_with_resource(RESOURCE_ID_DINNER);
    menu_icons[3] = gbitmap_create_with_resource(RESOURCE_ID_SNACKS);


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
    .select_click = breakfast_select_callback,
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



//click config provider
void config_provider(Window *window) {
    //button handlers
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  
}


static void convert_window_load(Window *window){
    // Get the root layer
  Layer *window_layer = window_get_root_layer(window);

  // Get the bounds of the window for sizing the text layer
  GRect bounds = layer_get_bounds(window_layer);

  // Create and Add to layer hierarchy:
  s_oz_layer = text_layer_create(GRect(5, 5, bounds.size.w, 30));
  s_cup_layer = text_layer_create(GRect(5,50,bounds.size.w,30));
  s_tbp_layer = text_layer_create(GRect(5, 100, bounds.size.w, 30));
  text_layer_set_text(s_oz_layer, "0 oz");
  layer_add_child(window_layer, text_layer_get_layer(s_oz_layer));
    text_layer_set_text(s_cup_layer, "0 cups");
  layer_add_child(window_layer, text_layer_get_layer(s_cup_layer));
    text_layer_set_text(s_tbp_layer, "0 tbsp");
  layer_add_child(window_layer, text_layer_get_layer(s_tbp_layer));
  
window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
  
}

static void recipe_window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, bounds.size.w, 2000);

  // Initialize the scroll layer
  s_scroll_layer = scroll_layer_create(bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  scroll_layer_set_click_config_onto_window(s_scroll_layer, window);

  // Initialize the text layer
  s_recipe_layer = text_layer_create(max_text_bounds);
  text_layer_set_text(s_recipe_layer, breakfast_recipes[0]);

  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  //text_layer_set_font(s_recipe_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(s_recipe_layer);
  text_layer_set_size(s_recipe_layer, max_size);
  scroll_layer_set_content_size(s_scroll_layer, GSize(bounds.size.w, max_size.h + 4));

  // Add the layers for display
  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_recipe_layer));

  // The inverter layer will highlight some text
  s_invert_layer = inverter_layer_create(GRect(0, 28, bounds.size.w, 28));
  scroll_layer_add_child(s_scroll_layer, inverter_layer_get_layer(s_invert_layer));
  
  layer_set_update_proc((Layer*)s_recipe_layer, update_text_layer);


  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));  
}
static void recipe_window_unload(Window *window){
   text_layer_destroy(s_recipe_layer);
  inverter_layer_destroy(s_invert_layer);
  scroll_layer_destroy(s_scroll_layer);
}

static void convert_window_unload(Window *window){
  text_layer_destroy(s_oz_layer);
  text_layer_destroy(s_cup_layer);
  text_layer_destroy(s_tbp_layer);
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
  

  s_convert_window = window_create();

   window_set_window_handlers(s_convert_window, (WindowHandlers) {
    .load = convert_window_load,
    .unload = convert_window_unload,
  });
  
    s_recipe_window = window_create();

   window_set_window_handlers(s_recipe_window, (WindowHandlers) {
    .load = recipe_window_load,
    .unload = recipe_window_unload,
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
