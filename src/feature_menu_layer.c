#include "pebble.h"

#define NUM_MENU_SECTIONS 2
#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 5

static Window *window;

char *routes[]={"Commuter Southbound",
 "Commuter Northbound",
 "Northwood",
"Bursley-Baits",
"Bursley-Baits (Nights)",
"Intercampus to East Campus"
"Intercampus to NIB",
"Mitchell-Glazier to Glazier and VA",
"KMS Shuttle",
"Oxford Shuttle",
"Diag to Diag express"
"Commuter Northbound (Nights)"
"Oxford Loop to Diag to Diag Express",
"North Campus",
"Night Owl",
"Bursley-Baits",
"Northwood",
"Oxford Shuttle"};

char *values[]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
"13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27",
"28", "29", "30"};

static int counter = 0;
static int *arrivalTimes;
static int *buses;
static char *title;


// This is a menu layer
// You have more control than with a simple menu layer
static MenuLayer *menu_layer;

// Menu items can optionally have an icon drawn with them
static GBitmap *menu_icons[NUM_MENU_ICONS];

static int current_icon = 0;

// You can draw arbitrary things in a menu item such as a background
static GBitmap *menu_background;


//////////////////////////////////////////
void out_sent_handler(DictionaryIterator *sent, void *context) {
   // outgoing message was delivered
 }


 void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
  APP_LOG(APP_LOG_LEVEL_DEBUG, "REASON: %i", reason);
 }

 void in_received_handler(DictionaryIterator *iter, void *context) {

      Tuple *text_tuple; 
      Tuple *tuple;
      Tuple *tuple2;
      Tuple *tuple3;
      int size = 0;
      int i = 0;
      switch(counter){

        case(0):
          text_tuple = dict_find(iter, 0);
            if(text_tuple){

              title = malloc(text_tuple->length);
              strncpy(title, text_tuple->value->cstring, text_tuple->length);
              APP_LOG(APP_LOG_LEVEL_DEBUG, "Title: %s", title);
            }

        break;
        case(1):

            tuple = dict_find(iter, 0);
            if(tuple){

              size = tuple->value->int32;
              APP_LOG(APP_LOG_LEVEL_DEBUG, "Route Size is %i", size);
            }
            

        break;
        case(2):

            tuple2 = dict_find(iter, 0);
            while (tuple2) {

               arrivalTimes[i] = tuple2->value->int32;

               tuple2 = dict_read_next(iter);
               i ++;
            }

            for(i = 0; i < 5; i ++){

                APP_LOG(APP_LOG_LEVEL_DEBUG, "%i is %i", i, arrivalTimes[i]);

            }

        break;
        case(3):

            tuple3 = dict_find(iter, 0);
            while (tuple3) {

               buses[i] = tuple3->value->int32;
               tuple3 = dict_read_next(iter);
               i ++;
            }

            for(i = 0; i < 5; i ++){

                APP_LOG(APP_LOG_LEVEL_DEBUG, "%i is %i", i, buses[i]);
                if(buses[i] >= 0 && buses[i] < 19){


                    
                }

            }

        APP_LOG(APP_LOG_LEVEL_DEBUG, "Title is %s", title);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Title is %s", title);   
        for(int i = 0; i < 5; i ++){

              APP_LOG(APP_LOG_LEVEL_DEBUG, "Bus is %s", routes[buses[i]]); 
          
        }       

        break;
        default:

            APP_LOG(APP_LOG_LEVEL_DEBUG, "Default");
        break;

      };

      counter ++;

 }


 void in_dropped_handler(AppMessageResult reason, void *context) {
   // incoming message dropped
  APP_LOG(APP_LOG_LEVEL_DEBUG, "REASON: %i", reason);
 }
///////////////////////////////////////////////////////


// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;

    case 1:
      return NUM_SECOND_MENU_ITEMS;

    default:
      return 0;
  }
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Closest Bus Stop");
      break;

    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Bus Locations");
      break;
  }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          if(!title)
            menu_cell_basic_draw(ctx, cell_layer, "Bus Stop Name", NULL, NULL);
          else
              menu_cell_basic_draw(ctx, cell_layer, title, NULL, NULL);
          break;
      }
      break;

    case 1:
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          if(!buses[0])
            menu_cell_basic_draw(ctx, cell_layer, "", NULL, NULL);
          else
            menu_cell_basic_draw(ctx, cell_layer, routes[buses[0]], values[arrivalTimes[0]], NULL);
          break;
        case 1:
          // This is a basic menu item with a title and subtitle
          if(!buses[1])
            menu_cell_basic_draw(ctx, cell_layer, "", NULL, NULL);
          else 
            menu_cell_basic_draw(ctx, cell_layer, routes[buses[1]], values[arrivalTimes[1]], NULL);
          break;

        case 2:
          // This is a basic menu item with a title and subtitle
          if(!buses[2])
            menu_cell_basic_draw(ctx, cell_layer, "", NULL, NULL);
          else
            menu_cell_basic_draw(ctx, cell_layer, routes[buses[2]], values[arrivalTimes[2]], NULL);

          break;

        case 3:
          // This is a basic menu item with a title and subtitle
          if(!buses[3])
            menu_cell_basic_draw(ctx, cell_layer, "", NULL, NULL);
          else
            menu_cell_basic_draw(ctx, cell_layer, routes[buses[3]], values[arrivalTimes[3]], NULL);
          break;

        case 4:
          // This is a basic menu item with a title and subtitle
          if(!buses[4])
            menu_cell_basic_draw(ctx, cell_layer, "", NULL, NULL);
          else
            menu_cell_basic_draw(ctx, cell_layer, routes[buses[4]], values[arrivalTimes[4]], NULL);
          break;
      }
  }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  switch (cell_index->row) {
    // This is the menu item with the cycling icon
    case 1:
      // Cycle the icon
      current_icon = (current_icon + 1) % NUM_MENU_ICONS;
      // After changing the icon, mark the layer to have it updated
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
  }

}

// This initializes the menu upon window load
void window_load(Window *window) {
  // Here we load the bitmap assets
  // resource_init_current_app must be called before all asset loading
  int num_menu_icons = 0;
  menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BIG_WATCH);
  menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_SECTOR_WATCH);
  menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BINARY_WATCH);

  // And also load the background
  menu_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BRAINS);

  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);

  // Cleanup the menu icons
  for (int i = 0; i < NUM_MENU_ICONS; i++) {
    gbitmap_destroy(menu_icons[i]);
  }

  // And cleanup the background
  gbitmap_destroy(menu_background);
}

int main(void) {
  window = window_create();

   arrivalTimes = (int *)malloc(sizeof(int)*5);
  buses = (int *)malloc(sizeof(int)*5);

  //set up messages
   app_message_register_inbox_received(in_received_handler);
   app_message_register_inbox_dropped(in_dropped_handler);
   app_message_register_outbox_sent(out_sent_handler);
   app_message_register_outbox_failed(out_failed_handler);

    const uint32_t inbound_size = 64;
   const uint32_t outbound_size = 64;
   app_message_open(inbound_size, outbound_size);

  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true /* Animated */);

  app_event_loop();

  window_destroy(window);
}
