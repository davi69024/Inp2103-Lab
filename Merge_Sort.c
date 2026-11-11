#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int transactionId;
    long timestamp;
    double amount;
} Transaction;

void merge(Transaction arr[], int left, int mid, int right) {
    int i = left, j = mid + 1, k = 0;
    int size = right - left + 1;
    Transaction *temp = malloc(size * sizeof(Transaction));

    if (temp == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    while (i <= mid && j <= right) {
        if (arr[i].timestamp <= arr[j].timestamp)
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (i = 0; i < size; i++) arr[left + i] = temp[i];
    free(temp);
}

void mergeSort(Transaction arr[], int left, int right) {
    int mid;
    if (left >= right) return;

    mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void printTransactions(const Transaction arr[], int size) {
    int i;
    printf("\n%-15s %-20s %-12s\n", "Transaction ID", "Timestamp", "Amount (USD)");
    printf("-------------------------------------------------\n");
    for (i = 0; i < size; i++)
        printf("%-15d %-20ld $%-11.2f\n", arr[i].transactionId, arr[i].timestamp, arr[i].amount);
}

int main(void) {
    int n, i;
    Transaction *arr;

    printf("=== TRANSACTION SORTING WITH MERGE SORT ===\nNumber of transactions: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Please enter a positive number of transactions.\n");
        return 1;
    }

    arr = malloc(n * sizeof(Transaction));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter each transaction as: ID TIMESTAMP AMOUNT\n");
    for (i = 0; i < n; i++) {
        printf("Transaction %d: ", i + 1);
        if (scanf("%d %ld %lf", &arr[i].transactionId, &arr[i].timestamp, &arr[i].amount) != 3) {
            printf("Invalid transaction data.\n");
            free(arr);
            return 1;
        }
    }

    printf("\nBefore sorting:");
    printTransactions(arr, n);
    mergeSort(arr, 0, n - 1);
    printf("\nAfter sorting by timestamp (ascending):");
    printTransactions(arr, n);

    free(arr);
    printf("\nAll dynamically allocated memory has been freed.\n");
    return 0;
}
