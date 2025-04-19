/*

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Linked list we will use to print frames from bottom to top of stack
typedef struct Node {
    unsigned long *frame_address;
    struct Node *next;
} Node_t;


/*
Prototypes
*/
void printStackFrames();
extern unsigned long *getFP();
extern unsigned long *getSP();
static void free_nodes(Node_t *head, Node_t *rsp_head);

void printStackFrames() {

    printf("\n");

    unsigned long *prev_frame = NULL;
    unsigned long *frame_address = getFP(); // Pointer to store the returned frame pointer from getFP()

    // Print the current %rbp value at the time when this function is called
    printf("Current %%rbp: %016lx\n", (uintptr_t)frame_address);
    printf("List of Frames: \n");

    Node_t *head = NULL;

    /*
    The next frame should always have a greater memory address than the previous one
    If it doesn't... we have reached the last frame
    */
    while (frame_address != NULL && frame_address > prev_frame) {

        Node_t *new_node = malloc(sizeof(Node_t));
        if (new_node == NULL) {
            exit(-1);
        }

        new_node->frame_address = frame_address;
        new_node->next = head;
        head = new_node;

        prev_frame = frame_address;
        frame_address = (unsigned long *)(*frame_address); // Get the next frame
    }

    // Create a temp node to iterate through linked list
    Node_t *temp = head;
    Node_t *bottom_frame = temp;

    // Print out list of frames
    while (temp != NULL) {

        printf("[%016lx] ", (uintptr_t)temp->frame_address); // Print the memory address of current frame pointer
        printf("%016lx\n", (*temp->frame_address));          // Deferenece the pointer to print its contents
        temp = temp->next;
    }

    printf("\n");

    // Get out stack pointer
    unsigned long *stack_pointer = getSP();
    printf("Current %%rsp: %016lx \n", (uintptr_t)stack_pointer);

    printf("Stack Dump: \n");

    unsigned long *prev_stack_pointer = NULL;

    // Create a second linked list
    // This time we go through incrementing rsp by 1 (8 Words)
    Node_t *rsp_head = NULL;

    while(stack_pointer != NULL && stack_pointer <= bottom_frame->frame_address) {

        Node_t *new_node = malloc(sizeof(Node_t));
        if(new_node == NULL) {
            fprintf(stderr, "Alloc to new node faild \n");
            exit(-1);
        }

        new_node->frame_address = stack_pointer;
        new_node->next = rsp_head;
        rsp_head = new_node;

        prev_stack_pointer = stack_pointer;
        stack_pointer = stack_pointer + 1;
    }

    Node_t *rsp_temp = rsp_head;
    Node_t *frame_temp = head;

    while(rsp_temp != NULL) {

        const char *rbp_label = "";
        const char *rip_label = "";
        const char *rsp_label = "";

    
        if(frame_temp != NULL && rsp_temp->frame_address == frame_temp->frame_address && frame_temp->next == NULL) {
            
            rbp_label = "%rbp -->";

        } else if (frame_temp != NULL && rsp_temp->frame_address == frame_temp->frame_address) {
            
            rbp_label = "%old rbp -->";
            frame_temp = frame_temp->next; 
        }



        if(frame_temp != NULL && (rsp_temp->frame_address + 1) == frame_temp->frame_address) {

            rip_label = "<-- %saved rip";
        } 

        if(rsp_temp != NULL && rsp_temp->next == NULL) {

            rsp_label = "%rsp -->";

            // Print current stack line
            printf("%-13s[%016lx] %016lx", rsp_label, (uintptr_t)rsp_temp->frame_address, *(rsp_temp->frame_address));

        } else {

            // Print current stack line
            printf("%-13s[%016lx] %016lx", rbp_label, (uintptr_t)rsp_temp->frame_address, *(rsp_temp->frame_address));
        }


        // Check if next rsp starts a frame
        if (frame_temp != NULL && rsp_temp->next != NULL && rsp_temp->next->frame_address == frame_temp->frame_address) {
           
            printf(" <-- %%saved rip \n \n");
        } else {

            printf("\n");
        }

        rsp_temp = rsp_temp->next;
    }


    // Free all nodes
    free_nodes(head, rsp_head);
}

/*
Params: Head of list of frame addresses
Free's all nodes in the linked list
*/
void free_nodes(Node_t *head, Node_t *rsp_head)
{

    // Walk the lists
    while (head != NULL) {
        Node_t *temp = head;
        head = head->next;
        free(temp);
    }

    while(rsp_head != NULL) {
        Node_t *temp = rsp_head;
        rsp_head = rsp_head->next;
        free(temp);
    }
}