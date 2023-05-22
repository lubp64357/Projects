import java.util.ArrayList;
import java.util.Random;

public class Bank {

    public String getName() {
        return name;
    }

    private String name;
    private ArrayList<User> users;
    private ArrayList<Account> accounts;

    public Bank(String name){
        this.name = name;
        this.accounts = new ArrayList<Account>();
        this.users = new ArrayList<User>();
    }


    /**
     * Generate a new unique user ID
     * @return the uuid
     */
    public String getNewUserUUID() {
        String uuid;
        Random rnd = new Random();
        int length = 6;
        boolean nonUnique;

        // looping until we get a new unique ID
        do {
            uuid = "";
            for (int c = 0; c < length; c++){
                uuid += ((Integer)rnd.nextInt(10)).toString();
            }
            // check to make sure its unique
            nonUnique = false;
            for (User u : this.users){
                if (uuid.compareTo(u.getUUID()) == 0){
                    nonUnique = true;
                    break;
                }
            }
        } while (nonUnique);
        return uuid;
    }


    public String getNewAccountUUID() {
        String uuid;
        Random rnd = new Random();
        int length = 10;
        boolean nonUnique;

        // looping until we get a new unique ID
        do {
            uuid = "";
            for (int c = 0; c < length; c++){
                uuid += ((Integer)rnd.nextInt(10)).toString();
            }
            // check to make sure its unique
            nonUnique = false;
            for (Account a : this.accounts){
                if (uuid.compareTo(a.getUUID()) == 0){
                    nonUnique = true;
                    break;
                }
            }
        } while (nonUnique);
        return uuid;
    }

    /**
     * Create a new user of a bank
     * @param firstName
     * @param lastName
     * @param pin
     * @return the User
     */
    public User addUser(String firstName, String lastName, String pin){
        User newUser = new User(firstName, lastName,pin, this);
        this.users.add(newUser);

        //create a savings account
        Account newAccount = new Account("Savings", newUser, this);

        //add account in the holders acc and bank acc
        this.accounts.add(newAccount);
        newUser.addAccount(newAccount);
        return newUser;
    }

    /**
     * Get the User object assosiated with the uID and pin, if they are valid
     * @param uuid
     * @param pin
     * @return the User, if the login is successful, or null unsuccessful
     */
    public User userLogin(String uuid, String pin){

        for(User u : this.users){
            if (u.getUUID().compareTo(uuid) == 0 && u.validatePin(pin) ){
                return u;
            }
        }

        return null;
    }

    public  Account addAccount(Account acc ){
        this.accounts.add(acc);
        return acc;
    }


}
