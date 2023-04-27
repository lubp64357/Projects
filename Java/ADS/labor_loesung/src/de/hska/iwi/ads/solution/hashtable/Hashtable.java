package de.hska.iwi.ads.solution.hashtable;
import de.hska.iwi.ads.dictionary.AbstractHashMap;

public class Hashtable<K extends Comparable<K>, V> extends AbstractHashMap<K, V> {

	public Hashtable(int capacity) {
		super(capacity);
		// TODO Auto-generated constructor stub
	}

	public V get(Object o) {
		@SuppressWarnings("unchecked")

		K key = (K) o;  // in Typ K casten
		Entry<K, V> val = find(key);
		if (val != null) {
			return val.getValue();
		} else {
			return null; // kein wert mit diesem Schlussel existiert
		}
	}

	public V put(K key, V value) { 

		Entry<K, V> entry = find(key);
		// bereits vorhanden
		if (entry != null) {
			
			return entry.setValue(value);
			
		}
		if (size + 1 >= hashtable.length) {
			// throw exception
			throw new de.hska.iwi.ads.dictionary.AbstractDictionary.DictionaryFullException();
		}
		size++;
		int hashindex = findEmpty(key);
		Entry<K, V> newEntry = new SimpleEntry<K, V>(key, value);
		if (hashindex == -10) {
			// throw exception
			throw new NullPointerException();
		}
		hashtable[hashindex] = newEntry;
		return null;
	}

	private Entry<K, V> find(K key) {

		int i = 0;
		int hashindex = 0;

		while (i < hashtable.length) {

			hashindex = (key.hashCode() + i * i) % hashtable.length; // quadratischen Sondierung
			if (hashtable[hashindex] != null && hashtable[hashindex].getKey().compareTo(key) == 0) {
				return hashtable[hashindex];
			}

			i++;
		}
		return null;
	}

	private int findEmpty(K key) {

		int i = 0;
		int hashindex = 0;

		while (i < hashtable.length) {

			hashindex = (key.hashCode() + i * i) % hashtable.length;
			if (hashtable[hashindex] == null || hashtable[hashindex].getKey() == null) {
				return hashindex;
			}
			i++;
		}
		return -10;

	}

}
