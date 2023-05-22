import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.security.MessageDigest;
public class User {



    private String firstName;
    private  String lastName;
    private String uudid;
    private byte pinHash[];
    private ArrayList<Account> accounts;

    /**
     *  Create a new user
     * @param firstName the users first name
     * @param lastName the users last name
     * @param pin the users pin
     * @param theBank the user is customer of theBank
     */
    public User(String firstName, String lastName, String pin, Bank theBank) {
        this.firstName = firstName;
        this.lastName = lastName;

        //hash the pin with the MD5 algorithm, for security
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            this.pinHash = md.digest(pin.getBytes());
        } catch (NoSuchAlgorithmException e) {
            System.err.println("error, caught NoSuchFAlgException");
            System.exit(1);
        }
        // get a new, unique universal ID for the user
        this.uudid = theBank.getNewUserUUID();
        this.accounts = new ArrayList<Account>();

        // print log message
        System.out.printf("New user %s, %s is the ID %s created. \n", this.lastName, this.firstName, this.uudid);
    }

    /**
     * add an account
     * @param account the acc to be added
     */
    public void addAccount(Account account) {
        this.accounts.add(account);
    }

    public String getUUID() {
        return this.uudid;
    }

    public boolean validatePin(String pin){
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            return MessageDigest.isEqual(md.digest(pin.getBytes()), this.pinHash);
        } catch (NoSuchAlgorithmException e) {
            System.err.println("error, caught NoSuchFAlgException");
            System.exit(1);
        }
        return false;
    }
    public String getFirstName() {
        return firstName;
    }

    public void printAccountsSummary() {
        System.out.printf("\n\n%s's accounts summary. \n", this.firstName);
        for (int a = 0; a< this.accounts.size();a++){
            System.out.printf("  %d) %s\n",a + 1, this.accounts.get(a).getSummaryLine() );
        }
        System.out.println();
    }

    public int numAccounts() {
        return this.accounts.size();
    }

    public void printAccTransactionHistory(int theAccIdx) {
        this.accounts.get(theAccIdx).printTransHistory();

    }

    public double getAccBalance(int fromAcc) {
        return this.accounts.get(fromAcc).getBalance();
    }

    public void addAccTransaction(int accIdx, double amount, String memo) {
        this.accounts.get(accIdx).addTransaction(amount, memo);
    }

    public String getAccUUID(int toAcc) {
        return this.accounts.get(toAcc).getUUID();
    }
}
