#!/bin/bash

# Initialize phonebook file if it doesn't exist
if [ ! -f phonebook.txt ]; then
    touch phonebook.txt
fi


while true; do
    # Display menu
    echo "Phone Book Management System"
    echo "1. Add a new name to phone book"
    echo "2. Display matches to a name or phone number"
    echo "3. Sort phone book by last name"
    echo "4. Delete an entry"
    echo "5. Quit"
    echo -n "Enter your choice (1-5): "
    read choice

    case $choice in
        1)  # Add new entry
            echo -n "Enter First Name: "
            read first_name
            echo -n "Enter Last Name: "
            read last_name
            echo -n "Enter Phone Number: "
            read phone

            # Add entry with tab separation
            echo -e "$first_name\t$last_name\t$phone" >> phonebook.txt
            echo "Entry added successfully!"
            ;;

        2)  # Search entries
            echo -n "Enter search term (name or phone number): "
            read search_term
            echo "Search Results:"
            echo -e "Name\tPhone"
            echo "------------------------"
            grep -i "$search_term" phonebook.txt
            ;;

        3)  # Sort by last name
            echo "Sorting phone book by last name..."
            sort -k2,2 -f phonebook.txt -o phonebook.txt
            echo "Phone book has been sorted!"
            echo -e "First Name\tLast Name\tPhone"
            echo "-----------------------------------"
            cat phonebook.txt
            ;;


        4)  # Delete entry
            echo -n "Enter name or phone number to delete: "
            read term

            if grep -iq "$term" phonebook.txt; then
                sed -i "/$term/Id" phonebook.txt
                echo "Deleted successfully!"
            else
                echo "No matching record found!"
            fi
            ;;

        5)  # Quit
            echo "Thank you for using Phone Book Management System"
            exit 0
            ;;

        *)  # Invalid choice
            echo "Invalid choice! Please enter a number between 1 and 5."
            ;;
    esac

    echo -e "\nPress Enter to continue..."
    read
    clear
done