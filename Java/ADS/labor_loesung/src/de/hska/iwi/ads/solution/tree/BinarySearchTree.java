package de.hska.iwi.ads.solution.tree;

import java.util.Map.Entry;

import de.hska.iwi.ads.dictionary.AbstractBinaryTree;

public class BinarySearchTree<K extends Comparable<K>, V> extends AbstractBinaryTree<K, V> {

	

	public V get(Object o) {
		@SuppressWarnings("unchecked")
		K key = (K) o;
		Node val = findNode(key);
		if (val != null) {
			return val.entry.getValue();
		} else {
			return null;
		}
	}

	public V put(K key, V value) {
		if (root == null) {
			root = new Node(key, value);
			size++;
			return null;
		}
		Node temp = findNode(key);
		if (temp != null) {
			V old = temp.entry.getValue();
			temp.entry.setValue(value);
			return old;
		}
		Node insert = new Node(key, value);
		temp = findParentToInsertNode(key);
		if (temp.entry.getKey().compareTo(key) > 0) {
			temp.left = insert;
		} else {
			temp.right = insert;
		}
		size++;
		return null;
	}
	
	private Node findNode(K key) {
		Node temp = root;
		while (temp != null) {
			if (temp.entry.getKey().compareTo(key) == 0) {
				return temp;
			} else if (temp.entry.getKey().compareTo(key) > 0) {
				temp = temp.left;
			} else {
				temp = temp.right;
			}
		}
		return null;

	}

	private Node findParentToInsertNode(K key) {
		Node parent = root;
		Node temp = root;
		while (temp != null) {
			if (temp.entry.getKey().compareTo(key) > 0) {
				parent = temp;
				temp = temp.left;
			} else {
				parent = temp;
				temp = temp.right;
			}
		}
		return parent;
	}

}