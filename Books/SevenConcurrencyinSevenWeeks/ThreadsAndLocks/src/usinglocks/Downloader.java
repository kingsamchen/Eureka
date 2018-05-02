package usinglocks;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.util.ArrayList;

public class Downloader extends Thread {
    private static final int BUF_SIZE = 1024;

    public interface ProgressListener {
        void onProgress(int p);
    }

    private ArrayList<ProgressListener> mListeners;
    private final Object mListenerMutex = new Object();

    private InputStream mInput;
    private OutputStream mOutput;

    Downloader(URL url, String saveFileName) throws IOException {
        mListeners = new ArrayList<>();
        mInput = url.openConnection().getInputStream();
        mOutput = new FileOutputStream(saveFileName);
    }

    public void addListener(ProgressListener listener) {
        synchronized (mListenerMutex) {
            mListeners.add(listener);
        }
    }

    public void removeListener(ProgressListener listener) {
        synchronized (mListenerMutex) {
            mListeners.remove(listener);
        }
    }

    private void notifyListeners(int p) {
        ArrayList<ProgressListener> listenersCopy = null;
        synchronized (mListenerMutex) {
            listenersCopy = (ArrayList<ProgressListener>) mListeners.clone();
        }

        for (ProgressListener listener : listenersCopy) {
            listener.onProgress(p);
        }
    }

    public void run() {
        int bytes_read = 0;
        int total_read = 0;

        byte[] buf = new byte[BUF_SIZE];

        try {
            while ((bytes_read = mInput.read(buf)) != -1) {
                mOutput.write(buf, 0, bytes_read);
                total_read += bytes_read;
                notifyListeners(total_read);
            }

            mOutput.flush();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
