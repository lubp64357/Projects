import java.util.Properties;
import javax.mail.Folder;
import javax.mail.Message;
import javax.mail.Session;
import javax.mail.Store;

public class Receive_Mail {
	public static void main(String[] args) throws Exception {
		fetchMail();
	}
	
	public static void fetchMail() {
		try {
			Properties props = new Properties();
			props.put("mail.pop3.host", "localhost");


			Session session = Session.getInstance(props, null);


			Store store = session.getStore("pop3");
			store.connect("localhost", "labrat", "kn1lab");

			Folder folder = store.getFolder("INBOX");
			folder.open(1);
			Message [] messages = folder.getMessages();

			for (int i = 0; i < messages.length; i++){

				System.out.println("message number:");
				System.out.println(messages[i].getMessageNumber());
				System.out.println("getFrom:");
				System.out.println(messages[i].getFrom());
				System.out.println("getSubject:");
				System.out.println(messages[i].getSubject());
				System.out.println("sendDate:");
				System.out.println(messages[i].getSentDate());
				System.out.println("getContent:");
				System.out.println(messages[i].getContent());



			}

		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
