import com.sun.security.jgss.GSSUtil;

import java.util.Scanner;

public class ATM {

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        Bank theBank = new Bank("Ljubo's Bank");

        User aUser = theBank.addUser("Ljubo", "Iliev", "1234");

        // add a checking account
        Account newAccount = new Account("Checking", aUser, theBank );
        aUser.addAccount(newAccount);
        theBank.addAccount(newAccount);

        User curUser;
        while (true){
            // stay in the login promt untill successful login
            curUser = ATM.mainMenuPrompt(theBank, sc);

            // stay in main menu until user quits
            ATM.printUserMenu(curUser, sc);
        }
    }

    public static void printUserMenu(User curUser, Scanner sc) {
        // print a summary of the user's accounts
        curUser.printAccountsSummary();

        int choice;

        do {
            System.out.printf("Welcome %s, what would you like to do?\n", curUser.getFirstName());
            System.out.println("  1) Show transaction history");
            System.out.println("  2) Withdraw");
            System.out.println("  3) Deposit");
            System.out.println("  4) Transfer");
            System.out.println("  5) Log Out");
            System.out.println("  6) Quit");
            System.out.print("Enter choice: ");
            choice = sc.nextInt();

            if (choice < 1 || choice > 6){
                System.out.println("Enter a valid number between 1-5!");
            }
        } while (choice < 1 || choice > 6);

        switch (choice){
            case 1:
                ATM.showTransactionHistory(curUser, sc);
                break;
            case 2:
                ATM.withdrawFunds(curUser, sc);
                break;
            case 3:
                ATM.depositFunds(curUser, sc);
                break;
            case 4:
                ATM.transferFunds(curUser, sc);
                break;
            case 5:
                sc.nextLine();
                break;
            case 6:
                System.exit(0);
        }

        // redisplay this menu unless user want to quit
        if (choice != 5) {
            ATM.printUserMenu(curUser, sc);
        }

    }

    private static void depositFunds(User curUser, Scanner sc) {
        int fromAcc;
        int toAcc;
        double amount;
        double accBal;
        String memo;

        // get acc to transfer from
        do {
            System.out.printf("Enter the number (1-%d) of the account\n"
                    + " to deposit in: ", curUser.numAccounts());
            fromAcc = sc.nextInt() - 1;
            if(fromAcc < 0 || fromAcc > curUser.numAccounts()){
                System.out.println("Invalid acc. Please try again.");
            }
        } while(fromAcc < 0 || fromAcc > curUser.numAccounts());

        accBal = curUser.getAccBalance(fromAcc);

        // get amount to transfer
        do {
            System.out.print("Enter the amount to transfer: ");
            amount = sc.nextDouble();
            if (amount < 0 ){
                System.out.println("Amount must be greater than zero");
            }
        } while(amount < 0);

        sc.nextLine();

        //get a memo
        System.out.println("Enter a memo: ");
        memo = sc.nextLine();

        curUser.addAccTransaction(fromAcc, amount, memo);
    }

    private static void withdrawFunds(User curUser, Scanner sc) {
        int fromAcc;

        double amount;
        double accBal;
        String memo;

        // get acc to withdraw from
        do {
            System.out.printf("Enter the number (1-%d) of the account\n"
                    + " to withdraw from: ", curUser.numAccounts());
            fromAcc = sc.nextInt() - 1;
            if(fromAcc < 0 || fromAcc > curUser.numAccounts()){
                System.out.println("Invalid acc. Please try again.");
            }
        } while(fromAcc < 0 || fromAcc > curUser.numAccounts());

        accBal = curUser.getAccBalance(fromAcc);



        do {
            System.out.printf("Enter the amount to withdraw (max: $%.02f): $", accBal);
            amount = sc.nextDouble();
            if (amount < 0 ){
                System.out.println("Amount must be greater than zero");
            } else if (amount > accBal)
            {
                System.out.printf("Amount must not be greater than account balance (%.02f)", accBal);
            }
        } while(amount < 0 || amount > accBal);

        sc.nextLine();

        //get a memo
        System.out.println("Enter a memo: ");
        memo = sc.nextLine();

        curUser.addAccTransaction(fromAcc, -1 * amount, memo);
    }

    private static void transferFunds(User curUser, Scanner sc) {
        int fromAcc;
        int toAcc;
        double amount;
        double accBal;

        // get acc to transfer from
        do {
            System.out.printf("Enter the number (1-%d) of the account\n"
                    + " to transfer from: ", curUser.numAccounts());
            fromAcc = sc.nextInt() - 1;
            if(fromAcc < 0 || fromAcc > curUser.numAccounts()){
                System.out.println("Invalid acc. Please try again.");
            }
        } while(fromAcc < 0 || fromAcc > curUser.numAccounts());

        accBal = curUser.getAccBalance(fromAcc);

        // get acc to transfer to
        do {
            System.out.printf("Enter the number (1-%d) of the account\n"
                    + " to transfer to: ", curUser.numAccounts());
            toAcc = sc.nextInt() - 1;
            if(toAcc < 0 || fromAcc > curUser.numAccounts()){
                System.out.println("Invalid acc. Please try again.");
            }
        } while(toAcc < 0 || fromAcc > curUser.numAccounts());

        // get amount to transfer
        do {
            System.out.printf("Enter the amount to transfer from (max: $%.02f): $", accBal);
            amount = sc.nextDouble();
            if (amount < 0 ){
                System.out.println("Amount must be greater than zero");
            } else if (amount > accBal)
            {
                System.out.printf("Amount must not be greater than account balance (%.02f)", accBal);
            }
        } while(amount < 0 || amount > accBal);

        // finally, do the transfer
        curUser.addAccTransaction(fromAcc, -1 * amount, String.format("Transfer to account %s",
                curUser.getAccUUID(toAcc)));

        curUser.addAccTransaction(toAcc, amount, String.format("Transfer to account %s",
                curUser.getAccUUID(fromAcc)));

    }


    public static User mainMenuPrompt(Bank theBank, Scanner sc) {
        String userID;
        String pin;
        User authUser;

        // promt the user for userID/pin combo until success
        do {
            System.out.printf("\n\nWelcome to %s\n\n",theBank.getName() );
            System.out.print("Enter user ID: ");
            userID = sc.nextLine();
            System.out.print("\nEnter pin: ");
            pin = sc.nextLine();

            // try to get the user obj corresponding to the combo

            authUser = theBank.userLogin(userID, pin);

            if (authUser == null){
                System.out.println("Incorrect user ID/pin combination. " +
                        "Please try again. ");
            }
        } while (authUser == null); // continue looping until successful login
        return authUser;
    }

    private static void showTransactionHistory(User curUser, Scanner sc) {
        int theAcc;
        do {
            System.out.printf("Enter a number between (1-%d) of the acc whose "
                    + "transaction you want to see: ",
                    curUser.numAccounts());
            theAcc = sc.nextInt() - 1;
            if (theAcc < 0 || theAcc >= curUser.numAccounts()){
                System.out.println("Invalid acc. Please try again.");
            }
        } while (theAcc < 0 || theAcc >= curUser.numAccounts());

        curUser.printAccTransactionHistory(theAcc);
    }
}
