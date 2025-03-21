#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 10
#define MAX_SEATS 50
#define ADMIN_PASS "admin123"
#define USER_PASS "user123"

#define ECONOMY_PRICE_FACTOR 0.8  // 20% cheaper
#define STANDARD_PRICE_FACTOR 1.0  // Base price
#define PREMIUM_PRICE_FACTOR 1.5   // 50% expensive

typedef struct {
    int id;
    char name[50];
    int seats[MAX_SEATS]; // 1 for booked, 0 for available
    int basePrice;
} Movie;

Movie movies[MAX_MOVIES];
int movie_count = 0;
int revenue = 0;

void loadMovies() {
    FILE *file = fopen("movies.dat", "rb");
    if (file) {
        fread(&movie_count, sizeof(int), 1, file);
        fread(movies, sizeof(Movie), movie_count, file);
        fclose(file);
    }
}

void saveMovies() {
    FILE *file = fopen("movies.dat", "wb");
    fwrite(&movie_count, sizeof(int), 1, file);
    fwrite(movies, sizeof(Movie), movie_count, file);
    fclose(file);
}

void saveRevenue() {
    FILE *file = fopen("revenue.dat", "wb");
    if (file) {
        fwrite(&revenue, sizeof(int), 1, file);
        fclose(file);
    }
}

void loadRevenue() {
    FILE *file = fopen("revenue.dat", "rb");
    if (file) {
        fread(&revenue, sizeof(int), 1, file);
        fclose(file);
    }
}

void adminMenu();
void userMenu();

void addMovie() {
    if (movie_count >= MAX_MOVIES) {
        printf("Movie list is full!\n");
        return;
    }
    printf("Enter movie name: ");
    scanf(" %s", &movies[movie_count].name);
    printf("Enter ticket price: ");
    scanf("%d", &movies[movie_count].basePrice);
    movies[movie_count].id = movie_count + 1;
    movie_count++;
    saveMovies();
    printf("Movie added successfully!\n");
}

void deleteMovie() {
    int id;
    printf("Enter movie ID to delete: ");
    scanf("%d", &id);
    if (id < 1 || id > movie_count) {
        printf("Invalid movie ID!\n");
        return;
    }
    for (int i = id - 1; i < movie_count - 1; i++) {
        movies[i] = movies[i + 1];
    }
    memset(&movies[movie_count - 1], 0, sizeof(Movie));
    for (int i = 0; i < movie_count; i++) {
        movies[i].id = i + 1;
    }
    movie_count--;
    saveMovies();
    printf("Movie deleted successfully!\n");
}

void viewMovies() {
    if (movie_count == 0) {
        printf("No movies available.\n");
        return;
    } else {
        printf("\n=================================\n");
        printf("| Available Movies |\n");
        printf("=================================\n");
        for (int i = 0; i < movie_count; i++) {
            printf("%d. %s - Rs.%d\n", movies[i].id, movies[i].name, movies[i].basePrice);
        }
        printf("=================================\n");
    }
}

void displaySeats(int movieIndex) {
    printf("\nSeat Matrix for %s:\n", movies[movieIndex].name);
    for (int i = 0; i < MAX_SEATS; i++) {
        int seatNumber = MAX_SEATS - i; // Display seat numbers in reverse order

        if (movies[movieIndex].seats[seatNumber - 1] == 0) {
            if (seatNumber < 10)
                printf("[ 0%d ]", seatNumber); // Proper spacing for alignment
            else
                printf("[ %d ]", seatNumber);
        } else {
            printf("[ XX ]"); // Booked seat
        }

        if ((i + 1) % 10 == 0) printf("\n"); // New row after every 10 seats
    }
    printf("\n");
}


int calculateSeatPrice(int basePrice, int seatNumber) {
    if (seatNumber <= 20)
        return basePrice * ECONOMY_PRICE_FACTOR;
    else if (seatNumber <= 40)
        return basePrice * STANDARD_PRICE_FACTOR;
    else
        return basePrice * PREMIUM_PRICE_FACTOR;
}

void bookTicket() {
    int id, numSeats, seat_no, totalCost = 0;
    viewMovies();
    printf("Enter movie ID to book: ");
    scanf("%d", &id);
    if (id < 1 || id > movie_count) {
        printf("Invalid movie ID!\n");
        return;
    }
    displaySeats(id - 1);
    printf("Enter number of seats to book: ");
    scanf("%d", &numSeats);
    int bookedSeats[numSeats],price[numSeats];
    for (int i = 0; i < numSeats; i++) {
        printf("Enter seat number %d: ", i + 1);
        scanf("%d", &seat_no);
        if (seat_no < 1 || seat_no > MAX_SEATS || movies[id - 1].seats[seat_no - 1]) {
            printf("Seat unavailable!\n");
            i--; // Retry for this seat
        } else {
            movies[id - 1].seats[seat_no - 1] = 1;
            price[i] = calculateSeatPrice(movies[id - 1].basePrice, seat_no);
            totalCost += price[i];
            bookedSeats[i] = seat_no;
        }
    }
    revenue += totalCost;
    saveRevenue();
    saveMovies();
    printf("\n========= Booking Confirmation =========\n");
    printf("Movie: %s\n", movies[id - 1].name);
    printf("Seats: ");
    for (int i = 0; i < numSeats; i++) {
        printf("%d (Rs.%d) ", bookedSeats[i], price[i]);
    }
    printf("\n---------------------------------------\n");
    printf("Total Amount: Rs.%d\n", totalCost);
    printf("========================================\n");
}

void cancelTicket() {
    viewMovies();
    int id, seat_no, refundAmount;
    printf("Enter movie ID for cancellation: ");
    scanf("%d", &id);
    if (id < 1 || id > movie_count) {
        printf("Invalid movie ID!\n");
        return;
    }
    displaySeats(id - 1);
    printf("Enter seat number to cancel (1-50): ");
    scanf("%d", &seat_no);
    if (seat_no < 1 || seat_no > MAX_SEATS || !movies[id - 1].seats[seat_no - 1]) {
        printf("Seat not booked!\n");
        return;
    }

    // Mark seat as available again
    movies[id - 1].seats[seat_no - 1] = 0;

    // Calculate refund amount based on seat category
    refundAmount = calculateSeatPrice(movies[id - 1].basePrice, seat_no);
    revenue -= refundAmount;
    saveRevenue();
    saveMovies();

    printf("Booking cancelled for %s, Seat %d. Refund: Rs.%d\n", movies[id - 1].name, seat_no, refundAmount);
}


void adminMenu() {
    char pass[20];
    printf("Enter Admin Password: ");
    scanf("%s", pass);
    if (strcmp(pass, ADMIN_PASS) != 0) {
        printf("Incorrect password!\n");
        return;
    }
    int choice;
    do {
        printf("\nAdmin Menu:\n1. View Movies\n2. Add Movie\n3. Delete Movie\n4. View Revenue\n5. Logout\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1: viewMovies(); break;
            case 2: addMovie(); break;
            case 3: deleteMovie(); break;
            case 4: printf("Total Revenue: Rs.%d\n", revenue); break;
        }
    } while (choice != 5);
}

void userMenu() {
    char pass[20];
    printf("Enter User Password: ");
    scanf("%s", pass);
    if (strcmp(pass, USER_PASS) != 0) {
        printf("Incorrect password!\n");
        return;
    }
    int choice;
    do {
        printf("\nUser Menu:\n1. View Movies\n2. Book Ticket\n3. Cancel Ticket\n4. Logout\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1: viewMovies(); break;
            case 2: bookTicket(); break;
            case 3: cancelTicket(); break;
        }
    } while (choice != 4);
}

int main() {
    loadMovies();
    loadRevenue(); 

    int choice;
    do {
        printf("========================================");
        printf("\nMain Menu:\n1. Admin Login\n2. User Login\n3. Exit\n");
        printf("========================================\n");
        printf("Enter:");
        scanf("%d", &choice);
        switch (choice) {
            case 1: adminMenu(); break;
            case 2: userMenu(); break;
        }
    } while (choice != 3);
    return 0;
}
