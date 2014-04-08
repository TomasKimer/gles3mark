package com.tomaskimer.gles3mark;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

// http://stackoverflow.com/questions/2109271/can-any-one-provide-me-example-of-two-line-list-item-in-android
// TODO own implementation http://www.androidhive.info/2012/02/android-custom-listview-with-image-and-text/
// or simple? http://stackoverflow.com/questions/6305899/custom-listview-android
public abstract class TwoLineArrayAdapter<T> extends ArrayAdapter<T> {
	private int mListItemLayoutResId;

	public TwoLineArrayAdapter(Context context, T[] ts) {
		this(context, R.layout.list_ranking, ts);  //  android.R.layout.two_line_list_item
	}

	public TwoLineArrayAdapter(Context context, int listItemLayoutResourceId, T[] ts) {
		super(context, listItemLayoutResourceId, ts);
		mListItemLayoutResId = listItemLayoutResourceId;
	}

	@Override
	public android.view.View getView(int position, View convertView, ViewGroup parent) {

		LayoutInflater inflater = (LayoutInflater)getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

		View listItemView = convertView;
		if (null == convertView) { 
			listItemView = inflater.inflate(mListItemLayoutResId, parent, false);
		}

		// The ListItemLayout must use the standard text item IDs.
		TextView lineOneView = (TextView)listItemView.findViewById(R.id.text1);  // android.R.id.text1
		TextView lineTwoView = (TextView)listItemView.findViewById(R.id.text2);

		T t = (T)getItem(position); 
		lineOneView.setText(lineOneText(t));
		lineTwoView.setText(lineTwoText(t));

		return listItemView;
	}

	public abstract String lineOneText(T t);
	public abstract String lineTwoText(T t);
}