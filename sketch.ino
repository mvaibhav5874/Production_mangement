#define CONSUMED_ITEMS_PER_SECOND 2
#define PRODUCED_ITEMS_PER_SECOND 6
#define MAX_PRODUCTION 60
#define MIN_PRODUCTION 1
#define MACHINE_PIN 3
#define MACHINE_START_PIN 12

int check_time = 1000;
int item_count = 0;
int total_production = 0;
int uptime = 0;
int downtime = 0;
int delta_production_count = 0;
bool production_stop = false;
bool display_stats = false;
bool display_warning = true;

void setup() {
  pinMode (MACHINE_PIN, OUTPUT);
  pinMode(MACHINE_START_PIN, INPUT);
  Serial.begin(9600);

  Serial.print("Items produced per second: ");
  Serial.println(PRODUCED_ITEMS_PER_SECOND);
  Serial.print("Items consumed per second: ");
  Serial.println(CONSUMED_ITEMS_PER_SECOND);
  Serial.print("Max Inventory (No. of items): ");
  Serial.println(MAX_PRODUCTION);
  Serial.print("MIN Inventory (No. of items): ");
  Serial.println(MIN_PRODUCTION);
  Serial.println();
  Serial.println();
}                                                       // initial pin mode

void loop() {
  if (digitalRead(MACHINE_START_PIN)) {
    if (item_count >= MAX_PRODUCTION) {
      Serial.println("Machine Turning Off");
      production_stop = true;
    } else if (item_count - MIN_PRODUCTION <= MIN_PRODUCTION){
      Serial.println("Machine Turning On");
      production_stop = false;
    }                                                   // Automating Machine Turning on and off

    if (!production_stop) {
      uptime++;
      delta_production_count = PRODUCED_ITEMS_PER_SECOND - CONSUMED_ITEMS_PER_SECOND;
      total_production += PRODUCED_ITEMS_PER_SECOND;
    } else {
      downtime++;
      delta_production_count = 0 - CONSUMED_ITEMS_PER_SECOND;
    }                                                   // Calculating production, uptime and downtime


    if (item_count + delta_production_count < 0) {
      item_count = 0;
    } else {
      item_count += delta_production_count;
    }                                                   // Enforcing Minimum item_count
    Serial.println("Items in Inventory: ");
    Serial.println(item_count);

    display_stats = true;
    display_warning = true;
  } else {

    if (item_count - CONSUMED_ITEMS_PER_SECOND <= 0) {
      item_count = 0;
    } else {
      item_count -= CONSUMED_ITEMS_PER_SECOND;
    }                                                   // Downfall while machine is offline

    if (display_stats) {
      Serial.print("Machine was turned on for: ");
      Serial.println(uptime + downtime);
      Serial.print("Machine Production Time: ");
      Serial.println(uptime);
      Serial.print("Machine Idle Time: ");
      Serial.println(downtime);
      Serial.print("TotalItemsProduced: ");
      Serial.println(total_production);
      uptime = downtime = total_production = 0;
      
      display_stats = false;                                
    }

    if (item_count == 0 && display_warning) {
      Serial.println("Inventory Depleted. Please turn on the switch.");
      display_warning = false;
    }
  }

  delay(check_time);
}
