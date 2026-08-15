package org.qtproject.MultiProg;

import java.util.HashMap;
import java.util.Iterator;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbRequest;
import android.util.Log;

import org.qtproject.R;

import android.os.Bundle;
import org.qtproject.qt5.android.bindings.QtActivity;

public class AndroidUSBTools extends QtActivity
{
	private static AndroidUSBTools m_instance;
	PendingIntent mPermissionIntent;
	UsbDevice device;
	static UsbManager manager;
	UsbDeviceConnection mConnection;
	private static final String ACTION_USB_PERMISSION = "com.mobilemerit.usbhost.USB_PERMISSION";

	/*
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
    	m_instance = this;
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        m_instance = null;
    }*/

	public AndroidUSBTools()
	{
		m_instance = this;

	}

	String fromNumber(int x)
	{
		return "salam4";
	}

	private static boolean getInstance()
	{
		if(m_instance  == null)
		{
			m_instance = new AndroidUSBTools();
			return false;
		}
		return true;
	}
	public static String CheckInfo(int x)
	{
		return "CheckInfo";

		/*if(!getInstance())
			return "instance is null";
                return m_instance.checkInfo();*/
	}

	public static String TestData(byte[] bytes)
	{
		return "byte 0:"+Integer.toString(bytes[0]);
		/*if(m_instance  == null)
        {
            m_instance = new AndroidUSBTools();
            m_instance.checkInfo();
            return " m_instance is null";
        }
        return m_instance.sendData(bytes, 10);*/
	}

	public static String SendData()
	{

		if(m_instance  == null)
		{
			m_instance = new AndroidUSBTools();
			m_instance.checkInfo();
			return " m_instance is null";
		}
		return m_instance.sendData();
	}

	//public String sendData(byte[] bytes, int len)
	public String sendData()
	{
		if(device==null)
			return "device is null";
		//UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);

		//byte[] bytes= new byte[1000];//{1,2,3,4,5,6,7,8,9,10};
		//byte[] bytes= {0,1,4,0,0,0,0,0,0,0};
		byte[] bytes = new byte[4];
		bytes[0]=0;
		bytes[1]=1;
		bytes[2]=2;
		bytes[3]=0;

		int TIMEOUT = 5;
		boolean forceClaim = true;
		if (device.getInterfaceCount() != 1) 
			return "could not find interface";

		UsbInterface intf = device.getInterface(0);
		if (intf.getEndpointCount() == 0)
			return "could not find endpoint";

		UsbEndpoint endpoint = intf.getEndpoint(1);
		if (endpoint.getType() != UsbConstants.USB_ENDPOINT_XFER_INT) 
			return  "endpoint is not interrupt type";


		UsbDeviceConnection connection = manager.openDevice(device);
		if(connection==null)
			return "connection  is null";
		if (connection != null && connection.claimInterface(intf, forceClaim))
		{
			connection.bulkTransfer(endpoint, bytes, bytes.length, TIMEOUT);
			//mConnection = connection;
			return "Sent SUCCESS";
			//Thread thread = new Thread(this);
			//thread.start();

			/*for(int i=0;i<1000;i++)
                        {
                            //connection.claimInterface(intf, forceClaim);
                            connection.bulkTransfer(endpoint, bytes, bytes.length, TIMEOUT);
                        }*/
		}
		return "Error";
	}



	public String checkInfo()
	{

		manager = (UsbManager) getSystemService(Context.USB_SERVICE);
		//this block required if you need to communicate to USB devices it's
		//take permission to device
		//if you want than you can set this to which device you want to communicate
		// ------------------------------------------------------------------
		mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(
				ACTION_USB_PERMISSION), 0);
		IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
		registerReceiver(mUsbReceiver, filter);
		// -------------------------------------------------------------------
		HashMap<String, UsbDevice> deviceList = manager.getDeviceList();
		Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
		String result = "";
		while (deviceIterator.hasNext())
		{
			device = deviceIterator.next();
			manager.requestPermission(device, mPermissionIntent);
			result = "\n" + "DeviceID: " + device.getDeviceId() + "\n"
					+ "DeviceName: " + device.getDeviceName() + "\n"
					+ "DeviceClass: " + device.getDeviceClass() + " - "
					+ "DeviceSubClass: " + device.getDeviceSubclass() + "\n"
					+ "VendorID: " + device.getVendorId() + "\n"
					+ "ProductID: " + device.getProductId() + "\n";
			if(device.getVendorId()==1240 && device.getProductId()==256)
				return result;
		}
		return "";
	}

	private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver()
	{
		public void onReceive(Context context, Intent intent)
		{
			String action = intent.getAction();
			if (ACTION_USB_PERMISSION.equals(action))
			{
				synchronized (this)
				{
					device = (UsbDevice) intent
							.getParcelableExtra(UsbManager.EXTRA_DEVICE);
					if (intent.getBooleanExtra(
							UsbManager.EXTRA_PERMISSION_GRANTED, false))
					{
						if (device != null)
						{
							// call method to set up device communication
						}
					} else
					{
						Log.d("ERROR", "permission denied for device " + device);
					}
				}
			}
		}
	};

}

