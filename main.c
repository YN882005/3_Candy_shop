/*Name: Youssef Nasser Farouk Elsayed Mohamed Kandiel*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CANDY_KINDS   6U
#define BASKET_MAX    8U
#define NAME_LEN     16U

typedef struct {
    char     name[NAME_LEN];
    uint16_t price;     
    uint16_t stock;      
    uint16_t sold;       
} Candy_t;

typedef struct {
    uint8_t  candyId;
    uint8_t  qty;
} Line_t;

static Candy_t shelf[CANDY_KINDS];
static Line_t  basket[BASKET_MAX];
static uint8_t basketLines;     
static uint32_t cashDrawer;

static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int read_int(const char *prompt, int *val) {
    printf("%s", prompt);
    if (scanf("%d", val) != 1) {
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    return 1;
}

static void openShop(void) {
    const char *names[CANDY_KINDS] = {
        "Gummy Bears", "Chocolate Bar", "Lollipop",
        "Sour Rings", "Jelly Beans", "Toffee Chew"
    };
    const uint16_t prices[CANDY_KINDS] = {150U, 250U, 75U, 125U, 200U, 100U};
    const uint16_t stocks[CANDY_KINDS] = {20U, 15U, 30U, 12U, 25U, 18U};

    for (size_t i = 0; i < CANDY_KINDS; ++i) {
        strncpy(shelf[i].name, names[i], NAME_LEN - 1);
        shelf[i].name[NAME_LEN - 1] = '\0';
        shelf[i].price = prices[i];
        shelf[i].stock = stocks[i];
        shelf[i].sold = 0U;
    }
    basketLines = 0U;
    cashDrawer = 0U;
    printf("Candy shop is open!\n");
}

static void showShelf(void) {
    printf("\n=== CANDY SHELF ===\n");
    printf("ID | %-15s | Price (pt) | Stock\n", "Name");
    printf("---|-----------------|------------|---------\n");
    for (size_t i = 0; i < CANDY_KINDS; ++i) {
        if (shelf[i].stock == 0) {
            printf("%2zu | %-15s | %10u | SOLD OUT\n",
                   i, shelf[i].name, (unsigned int)shelf[i].price);
        } else {
            printf("%2zu | %-15s | %10u | %u\n",
                   i, shelf[i].name, (unsigned int)shelf[i].price, (unsigned int)shelf[i].stock);
        }
    }
}

static uint32_t basketTotal(void) {
    uint32_t total = 0U;
    for (size_t i = 0; i < basketLines; ++i) {
        uint8_t id = basket[i].candyId;
        total += (uint32_t)shelf[id].price * (uint32_t)basket[i].qty;
    }
    return total;
}

static void showBasket(void) {
    if (basketLines == 0) {
        printf("\nBasket is empty.\n");
        return;
    }
    printf("\n=== CURRENT BASKET ===\n");
    printf("Line | %-15s | Qty | Price | Subtotal\n", "Name");
    printf("-----|-----------------|-----|-------|---------\n");
    for (size_t i = 0; i < basketLines; ++i) {
        uint8_t id = basket[i].candyId;
        uint32_t sub = (uint32_t)shelf[id].price * (uint32_t)basket[i].qty;
        printf("%4zu | %-15s | %3u | %5u | %6u pt\n",
               i + 1, shelf[id].name, (unsigned int)basket[i].qty,
               (unsigned int)shelf[id].price, (unsigned int)sub);
    }
    printf("-----------------------------------------------\n");
    printf("Total Amount: %u pt\n", (unsigned int)basketTotal());
}

static void addToBasket(void) {
    showShelf();
    int id_input = 0, qty_input = 0;
    if (!read_int("Enter Candy ID (0-5): ", &id_input) || id_input < 0 || id_input >= (int)CANDY_KINDS) {
        printf("Invalid Candy ID.\n");
        return;
    }
    if (!read_int("Enter Quantity: ", &qty_input) || qty_input <= 0) {
        printf("Invalid quantity.\n");
        return;
    }

    uint8_t id = (uint8_t)id_input;
    uint8_t qty = (uint8_t)qty_input;
    uint8_t currentInBasket = 0U;
    int existingLine = -1;

    for (size_t i = 0; i < basketLines; ++i) {
        if (basket[i].candyId == id) {
            currentInBasket = basket[i].qty;
            existingLine = (int)i;
            break;
        }
    }

    if ((uint32_t)currentInBasket + qty > (uint32_t)shelf[id].stock) {
        printf("Not enough stock! Shelf only has %u left (%u already in basket).\n",
               (unsigned int)shelf[id].stock, (unsigned int)currentInBasket);
        return;
    }

    if (existingLine >= 0) {
        basket[existingLine].qty += qty;
        printf("Updated quantity for %s to %u.\n", shelf[id].name, (unsigned int)basket[existingLine].qty);
    } else {
        if (basketLines >= BASKET_MAX) {
            printf("Basket is full! Cannot add more distinct items.\n");
            return;
        }
        basket[basketLines].candyId = id;
        basket[basketLines].qty = qty;
        basketLines++;
        printf("Added %s (x%u) to basket.\n", shelf[id].name, (unsigned int)qty);
    }
}

static void removeFromBasket(void) {
    if (basketLines == 0) {
        printf("Basket is already empty.\n");
        return;
    }
    showBasket();
    int line = 0;
    if (!read_int("Enter Line number to remove: ", &line) || line < 1 || line > (int)basketLines) {
        printf("Invalid line number.\n");
        return;
    }

    size_t idx = (size_t)(line - 1);
    for (size_t i = idx; i < basketLines - 1; ++i) {
        basket[i] = basket[i + 1];
    }
    basketLines--;
    printf("Line %d removed from basket.\n", line);
}

static void giveChange(uint32_t change) {
    if (change == 0) {
        printf("No change, thank you!\n");
        return;
    }
    printf("Change to return (%u pt):\n", (unsigned int)change);
    const uint32_t coins[] = {500U, 200U, 100U, 50U, 25U};
    const size_t num_coins = sizeof(coins) / sizeof(coins[0]);

    for (size_t i = 0; i < num_coins; ++i) {
        uint32_t count = change / coins[i];
        if (count > 0) {
            printf("  %u x %u pt coin(s)\n", (unsigned int)count, (unsigned int)coins[i]);
            change %= coins[i];
        }
    }

    if (change > 0) {
        printf("  Note: Remaining %u pt cannot be given (smallest coin is 25 pt).\n", (unsigned int)change);
    }
}

static void checkout(void) {
    uint32_t total = basketTotal();
    if (total == 0) {
        printf("Basket is empty! Nothing to checkout.\n");
        return;
    }

    showBasket();
    int payment_input = 0;
    if (!read_int("Enter money handed over (piastres): ", &payment_input) || payment_input < 0) {
        printf("Invalid payment amount.\n");
        return;
    }

    uint32_t payment = (uint32_t)payment_input;
    if (payment < total) {
        printf("Not enough money! You need %u pt, but only gave %u pt. Basket kept untouched.\n",
               (unsigned int)total, (unsigned int)payment);
        return;
    }

    for (size_t i = 0; i < basketLines; ++i) {
        uint8_t id = basket[i].candyId;
        uint8_t qty = basket[i].qty;
        shelf[id].stock -= qty;
        shelf[id].sold += qty;
    }

    cashDrawer += total;
    uint32_t change = payment - total;
    giveChange(change);

    basketLines = 0U;
    printf("Transaction complete! Thank you.\n");
}

static uint8_t bestSeller(void) {
    uint8_t best = 0U;
    uint16_t maxSold = shelf[0].sold;
    for (size_t i = 1; i < CANDY_KINDS; ++i) {
        if (shelf[i].sold > maxSold) {
            maxSold = shelf[i].sold;
            best = (uint8_t)i;
        }
    }
    return best;
}

static void dayReport(void) {
    uint32_t totalSold = 0U;
    for (size_t i = 0; i < CANDY_KINDS; ++i) {
        totalSold += shelf[i].sold;
    }

    uint8_t bestIdx = bestSeller();

    printf("\n=== END OF DAY REPORT ===\n");
    printf("Cash in Drawer: %u pt\n", (unsigned int)cashDrawer);
    printf("Total Candies Sold: %u\n", (unsigned int)totalSold);
    if (totalSold > 0) {
        printf("Best Seller: %s (%u sold)\n", shelf[bestIdx].name, (unsigned int)shelf[bestIdx].sold);
    } else {
        printf("Best Seller: None sold today\n");
    }

    printf("Sold Out Items:\n");
    int anySoldOut = 0;
    for (size_t i = 0; i < CANDY_KINDS; ++i) {
        if (shelf[i].stock == 0) {
            printf(" - %s\n", shelf[i].name);
            anySoldOut = 1;
        }
    }
    if (!anySoldOut) {
        printf(" - None\n");
    }
}

static void print_menu(void) {
    printf("\n--- CANDY SHOP MENU ---\n");
    printf("1. View Shelf\n");
    printf("2. View Basket\n");
    printf("3. Add Candy to Basket\n");
    printf("4. Remove Item from Basket\n");
    printf("5. Checkout\n");
    printf("6. End of Day Report\n");
    printf("7. Exit\n");
}

static int handle_menu(void) {
    print_menu();
    int choice = 0;
    if (!read_int("Select an option (1-7): ", &choice)) {
        printf("Invalid choice.\n");
        return 1;
    }

    switch (choice) {
        case 1: showShelf(); break;
        case 2: showBasket(); break;
        case 3: addToBasket(); break;
        case 4: removeFromBasket(); break;
        case 5: checkout(); break;
        case 6: dayReport(); break;
        case 7: printf("Closing shop for today.\n"); return 0;
        default: printf("Invalid choice. Select between 1 and 7.\n"); break;
    }
    return 1;
}

static void run_app(void) {
    openShop();
    int running = 1;
    do {
        running = handle_menu();
    } while (running);
}

int main(void) {
    run_app();
    return 0;
}