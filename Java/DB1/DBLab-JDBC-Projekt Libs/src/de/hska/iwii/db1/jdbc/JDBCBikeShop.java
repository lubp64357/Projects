package de.hska.iwii.db1.jdbc;

import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Properties;

/**
 * Diese Klasse ist die Basis für Ihre Lösung. Mit Hilfe der Methode
 * reInitializeDB können Sie die beim Testen veränderte Datenbank
 * wiederherstellen.
 */
public class JDBCBikeShop {
	private final String user = "g03";
	private final String pass = "GpHbGpkXYC";
	Connection connection;

	public static void main(String[] args) {
		JDBCBikeShop db = new JDBCBikeShop();
		db.openConnection();
		reInitializeDB(db.connection);
//		db.closeConnection();
//		db.getInfoForDb();
		db.readStatement("SELECT persnr, name, ort, aufgabe FROM personal");
//		db.readStatement("SELECT * FROM kunde");
//		String filter = "Rafa%";
//		db.readStatement("SELECT k.name AS kunde, k.nr AS knr, l.name AS lieferant, l.nr AS lnr FROM kunde k " +
//		"JOIN AUFTRAG a ON a.kundnr = k.nr " + 
//		"JOIN AUFTRAGSPOSTEN  ap ON ap.auftrnr = a.auftrnr " + 
//		"JOIN TEILESTAMM t ON t.teilnr = ap.teilnr " +
//		"JOIN LIEFERUNG lg ON lg.teilnr = t.teilnr " +
//		"JOIN LIEFERANT l ON l.nr = lg.liefnr " +
//		"WHERE k.name LIKE ?", filter);
		

	}

	/**
	 * Aufagebe 4.1
	 * 
	 * Verbindung mit Datenbank erstellen
	 */
	public void openConnection() {
		try {
			Class.forName("org.postgresql.Driver");
			Properties props = new Properties();
			props.put("user", this.user);
			props.put("password", this.pass);
			this.connection = DriverManager.getConnection("jdbc:postgresql://datenbanken1.ddns.net:3690/g03", props);

		} catch (Exception E) {
			System.err.println("Cannot find connection for PostgreSQL.");

		}
	}

	/**
	 * Aufagebe 4.1
	 * 
	 * Schliessen der Verbindung
	 * 
	 * @throws SQLException
	 */

	public void closeConnection() {
		try {
			this.connection.close();
			System.out.println("Database connection closed!");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Aufgabe 4.1 Ausgabe Infos ueber die Datenbank
	 */
	public void getInfoForDb() {
		try {
			System.out.println("Product Name: " + this.connection.getMetaData().getDatabaseProductName());
			System.out.println("Driver Name: " + this.connection.getMetaData().getDriverName());
			System.out.println("Driver Version: " + this.connection.getMetaData().getDriverVersion());
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	/**
	 * Aufgabe 4.2 Read statement formatted
	 * 
	 * @param statement SQL statement
	 * @return auf der Konsole formatierte Ausgabe
	 */
	public void readStatement(String statement) {
		try {
			
			Statement stmt = connection.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_UPDATABLE);
			ResultSet resultSet = stmt.executeQuery(statement);
			ResultSetMetaData metaData = resultSet.getMetaData();

			ArrayList<String> colNames = new ArrayList<>();
			ArrayList<String> colTypes = new ArrayList<>();
			ArrayList<Integer> colWidth = new ArrayList<>();
			int countColumns = metaData.getColumnCount();

			for (int i = 1; i < countColumns + 1; i++) {
				colNames.add(metaData.getColumnName(i));
				colTypes.add(metaData.getColumnTypeName(i));
				colWidth.add(metaData.getColumnDisplaySize(i));
			}
			
			//pruefen ob die Breite von Name oder Typ ist breiter als die Datenbreite
			for(int i = 0; i < countColumns; i++) {
				int size = colWidth.get(i);
				size = Math.max(size, colNames.get(i).length());
				size = Math.max(size, colTypes.get(i).length());
				colWidth.set(i, size);
			}
			while(resultSet.next()) {
				for(int i = 0; i < countColumns; i++) {
					int size = 0;
					size = Math.max(colWidth.get(i), resultSet.getString(i+1).length());
					colWidth.set(i, size);
				}
			}
			
			resultSet.beforeFirst();

			// Ausgabe von column Namen
			for (int i = 0; i < countColumns; i++) {
				String result = String.format("%-" + colWidth.get(i) + "s", colNames.get(i));
				System.out.print(result + "|");
			}
			System.out.println();

			// Ausgabe column typen
			for (int i = 0; i < countColumns; i++) {
				String result = String.format("%-" + colWidth.get(i) + "s", colTypes.get(i));
				System.out.print(result + "|");
			}
			System.out.println();

			// Ausgabe 1 Linie zw. den Typen und Daten
			for (int i = 0; i < countColumns; i++) {
				String result = String.format("%-" + colWidth.get(i) + "s", "-").replace(' ', '-');
				System.out.print(result + "+");
			}
			System.out.println();

			// Ausgabe Daten
			while (resultSet.next()) {
				for (int i = 0; i < countColumns; i++) {
					String result = String.format("%-" + colWidth.get(i) + "s", resultSet.getString(i + 1));
					System.out.print(result + "|");
				}
				System.out.println();
			}
			System.out.println();

			resultSet.close();
			stmt.close();

		} catch (Exception e) {

		}

	}
	/**
	 * Aufgabe 4.3 Read prepared statement
	 *  Same as 4.2 only filter added as prepared statement
	 *  to be able to bind value to query with ? 
	 * 
	 * @param statement SQL statement
	 * @param filter String to be added
	 * @return auf der Konsole formatierte Ausgabe
	 */
	public void readStatement(String statement, String filter) {
		try {
			
			PreparedStatement stmt = connection.prepareStatement(statement,ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_UPDATABLE);
			stmt.setString(1, filter);
			ResultSet resultSet = stmt.executeQuery();
			ResultSetMetaData metaData = resultSet.getMetaData();

			ArrayList<String> colNames = new ArrayList<>();
			ArrayList<String> colTypes = new ArrayList<>();
			ArrayList<Integer> colWidth = new ArrayList<>();
			int countColumns = metaData.getColumnCount();

			for (int i = 1; i < countColumns + 1; i++) {
				colNames.add(metaData.getColumnName(i));
				colTypes.add(metaData.getColumnTypeName(i));
				colWidth.add(metaData.getColumnDisplaySize(i));
			}
			
			//pruefen ob die Breite von Name oder Typ ist breiter als die Datenbreite
			for(int i = 0; i < countColumns; i++) {
				int size = colWidth.get(i);
				size = Math.max(size, colNames.get(i).length());
				size = Math.max(size, colTypes.get(i).length());
				colWidth.set(i, size);
			}
			
			resultSet.beforeFirst();

			// Ausgabe von column Namen
			for (int i = 0; i < countColumns; i++) {
				String result = String.format("%-" + colWidth.get(i) + "s", colNames.get(i));
				System.out.print(result + "|");
			}
			System.out.println();

			// Ausgabe column typen
			for (int i = 0; i < countColumns; i++) {
				String result = String.format("%-" + colWidth.get(i) + "s", colTypes.get(i));
				System.out.print(result + "|");
			}
			System.out.println();

			// Ausgabe 1 Linie zw. den Typen und Daten
			for (int i = 0; i < countColumns; i++) {
				String result = String.format("%-" + colWidth.get(i) + "s", "-").replace(' ', '-');
				System.out.print(result + "+");
			}
			System.out.println();

			// Ausgabe Daten
			while (resultSet.next()) {
				for (int i = 0; i < countColumns; i++) {
					String result = String.format("%-" + colWidth.get(i) + "s", resultSet.getString(i + 1));
					System.out.print(result + "|");
				}
				System.out.println();
			}
			System.out.println();

			resultSet.close();
			stmt.close();

		} catch (Exception e) {

		}

	}

	/**
	 * Stellt die Datenbank aus der SQL-Datei wieder her. - Alle Tabllen mit Inhalt
	 * ohne Nachfrage löschen. - Alle Tabellen wiederherstellen. - Tabellen mit
	 * Daten füllen.
	 * <p>
	 * Getestet mit MsSQL 12, MySql 8.0.8, Oracle 11g, Oracle 18 XE, PostgreSQL 14.
	 * <p>
	 * Das entsprechende Sql-Skript befindet sich im Ordner ./sql im Projekt.
	 * 
	 * @param connection Geöffnete Verbindung zu dem DBMS, auf dem die
	 *                   Bike-Datenbank wiederhergestellt werden soll.
	 */
	public static void reInitializeDB(Connection connection) {
		try {
			System.out.println("\nInitializing DB.");
			connection.setAutoCommit(true);
			String productName = connection.getMetaData().getDatabaseProductName();
			boolean isMsSql = productName.equals("Microsoft SQL Server");
			Statement statement = connection.createStatement();
			int numStmts = 0;

			// Liest den Inhalt der Datei ein.
			String[] fileContents = new String(Files.readAllBytes(Paths.get("sql/Bike.sql")), StandardCharsets.UTF_8)
					.split(";");

			for (String sqlString : fileContents) {
				try {
					// Microsoft kenn den DATE-Operator nicht.
					if (isMsSql) {
						sqlString = sqlString.replace(", DATE '", ", '");
					}
					statement.execute(sqlString);
					System.out.print((++numStmts % 80 == 0 ? "/\n" : "."));
				} catch (SQLException e) {
					System.out.print("\n" + sqlString.replace('\n', ' ').trim() + ": ");
					System.out.println(e.getMessage());
				}
			}
			statement.close();
			System.out.println("\nBike database is reinitialized on " + productName + "\nat URL "
					+ connection.getMetaData().getURL());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	/**
	 * // Add new customer (kunde)
			String einfuegenKunde = "INSERT INTO kunde (nr, name, strasse, plz, ort, sperre) VALUES (?, ?, ? ,? ,?, ?)"; 	
			PreparedStatement stmt1 = connection.prepareStatement(einfuegenKunde);
			stmt1.setInt(1,999);
			stmt1.setString(2,"Hochschule");
			stmt1.setString(3,"Moltkestraße");
			stmt1.setInt(4,76133);
			stmt1.setString(5,"Karlsruhe");
			stmt1.setInt(6,0);
			stmt1.execute();
			stmt1.close();
	 */
}
