# Scryt-Masternode-Setup-Guide
 
<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Logo.png">




## Introduction

This guide is for a single masternode, on a Ubuntu 16.04 64-bit server (VPS) and Ubuntu 18.04 64-bit server (VPS) running headless and will be controlled from the wallet on your local computer (Control wallet). The wallet on the the VPS will be referred to as the Remote wallet.

You will need your masternode server details for progressing through this guide including the public IP address.

### VPS Server Recommendations ###

[Digital Ocean](https://www.digitalocean.com/products/droplets/)

[Vultr](https://www.vultr.com/products/cloud-compute/#compute)

**For either of the above VPS services, the economical $5 plan will be sufficient for your masternode.**

---

First the basic requirements:

1. 15,000 SCY
1. A main computer (your everyday computer) – This will run the SCRYT Core wallet, hold your collateral 15,000 SCY and can be opened and closed without affecting the masternode.
1. Masternode Server (VPS Suggested – The wallet daemon that will be on 24/7).
1. A unique IP address for your VPS / Remote wallet.

**For security reasons, you’re are going to need a different IP for each masternode you plan to host.**


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Available-balance.png">

## Configuration

**Step 1:** Using the control wallet, enter the debug console `Tools > Debug console` and type the following command:

<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Debug-console.png">

```
masternode genkey
```
<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/masternode-genkey.png">

*(This will be the masternode’s privkey variable. We will need this later…)*



**Step 2:** Using the control wallet, enter the following command:

```
getnewaddress masternode01
```

<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/getnewaddress01.png">


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/getnewaddress.png">


**Step 3:** Still in the control wallet, send 15,000 SCY to the wallet address you generated in Step 2. 

Be 100% sure that you entered the address correctly. You can verify this when you paste the address into the **"Pay To:"** field, the label will auto populate with the name you chose. 

Also make sure this is exactly **15,000 SCY**; No less, no more.

**Be absolutely sure the send to address is copied correctly and then check it again. We cannot help you if you send 15,000 SCY to an incorrect address.**


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Collateral-send.png">


Please allow at least 1 block confirmation to complete before moving on.

**Step 4:** Still in the control wallet, enter the command into the console:

```
masternode outputs
```

<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/masternode_outputs.png">


*This gets the proof of transaction of sending 15,000 SCY*

**Step 5:** Still on the main computer, go into the SCY data directory:

OS | Path to SCRYT
------------ | -------------
Windows | `%Appdata%/Roaming/Scryt/`
macOS | `~/Library/Application\ Support/Scryt/`
Linux | `~/.scrytcore/`

Find the masternode.conf file, edit it in your favorite text editor and add the following line to it:

```
<Name of Masternode(Use the name you entered earlier for simplicity)> <Unique VPS Public IP address>:42122 <The result of Step 1> <Result of Step 4> <The number after the long line in Step 4>
```

Example:

```
masternode01 127.0.0.2:6006 93HaYBVUCYjEMeeH1Y4sBGLALQZE1Yc1K64xiqgX37tGBDQL8Xg 2bcd3c84c84f87eaa86e4e56834c92927a07f9e18718810b92e0d0324456a67c 0
```

Substitute with your own values without the "<>"s.


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/masternode.conf.png">


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/masternode.conf3.png">




Lastly, close the control wallet and open again to load the new configuration file.

## VPS Remote Wallet Install

Install the latest version of the Scryt Core wallet onto your masternode. The latest version can be found here: [Scryt Core Releases](https://github.com/SCRYT-COIN/scryt/releases).

**Step 1:** Log in to your VPS via SSH:

```
cd ~
```

**Step 2:** From your home directory, download the latest version from the Scryt GitHub repository:

For Linux 16.04:
```
wget https://github.com/SCRYT-COIN/scryt/releases/download/v4.0.1/Linux16.04-64bits.zip
```
For Linux Obuntu 18.04:
```
wget https://github.com/SCRYT-COIN/scryt/releases/download/v4.0.1/Linux18.04-64bits.zip
```

Always check the releases page for the latest version and update the URL to reflect the most current version.

**Step 3:** Unzip & Extract:

For Linux Obuntu 16.04:

```
unzip Linux16.04-64bits.zip
```

For Linux Obuntu 18.04:
```
unzip Linux18.04-64bits.zip
```

**Step 4:** Copy the files to the local bin. **Requires sudo**

```
sudo cp scrytd /usr/local/bin
sudo cp scryt-cli /usr/local/bin
```

**Step 5:** Note: If this is the first time running the wallet in the VPS, you’ll need to attempt to start the wallet:

```
scrytd --daemon
```

**Step 6:** Stop the daemon after the blockchain downloads:

You can verify you have the entire blockchain by comparing the results of `scryt-cli getinfo` with the [Scryt Block Explorer](https://scrytexplorer.com/)

Once verified:

```
scryt-cli stop
```

**Step 7:** Navigate to the scrytcore data directory:

```
cd ~/.scrytcore
```

**Step 8:** Open the configuration file by typing:

```
nano scryt.conf
```

**Step 9:** Make the config look like this with your values:

```
rpcuser=long_random_username
rpcpassword=longer_random_password
rpcallowip=127.0.0.1
server=1
daemon=1
logtimestamps=1
maxconnections=256
masternode=1
bind=Your VPS unique public ip address
port=6006
externalip=Your VPS unique public ip address
masternodeaddr=Your VPS unique public ip:6006
masternodeprivkey=Result of Step 1
```

*Make sure to replace rpcuser and rpcpassword with your own.*



<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Remote_scryt.conf.png">


**Step 10:** Save and exit the file:

```
Ctr+x to exit and press Y to save changes and press enter to close
```

**Please be sure to have port 6006 open on your server firewall if applicable for your control wallet to be able start the masternode remotely.**

## Start the Masternode

**Step 1:** Navigate back to the Scryt core VPS server:


**Step 2:** Start the wallet daemon:

```
scrytd --daemon
```


**Make sure your wallet synchronizing complete before start masternode:**


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Sync-data.png">


<img src="https://github.com/captain-8421/Scryt-Masternode-Setup-Guide/blob/guide-image/Sync.png">


**Step 3:** From the Control wallet debug console:

```
masternode start-alias masternode01
```

Where "masternode01" is the name of your masternode alias we shown in example,you can choose yours.


**The following should appear.**

```
{
  "overall": "Successfully started 1 masternodes, failed to start 0, total 1",
  "detail": [
    {
      "alias": "MN1",
      "result": "successful",
      "error": ""
    }
  ]
}

```

Or start yous masternode using **Initialize masternode** From the Masternode Tab

<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/start_masternode0.png">


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Start-masternode.png">


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/Successfully_started.png">


**Step 4:** Use the following command to check status:

```
scryt-cli masternode status
```

You should see something like:

```
{
  "txhash": "2bcd3c84c84f87eaa86e4e56834c92927a07f9e18718810b92e0d0324456a67c",
  "outputidx": 0,
  "netaddr": "127.0.0.1:6006",
  "addr": "MGEonPLWN1YAxZnpPj4iTsiWzZxPbSN9XP",
  "status": 4,
  "message": "Masternode successfully started"
}
```
Before **ENABLED** STATUS, your masternode will remain in **PRE_ENABLED** status for some minutes.


<img src="https://raw.githubusercontent.com/captain-8421/Scryt-Masternode-Setup-Guide/guide-image/PRE_ENABLED.png">




If you see status Not capable masternode: Hot node, waiting for remote activation, you need to wait a bit longer for the blockchain to reach consensus. It's possible it may take 60 to 120 minutes before the activation can be done. You can also try restarting the VPS wallet `scryt-cli stop` and then `scrytd --daemon` and trying the `masternode start-alias masternode01` command again local wallet.

Scryt core Masternode Setup is Complete!




## Tearing down a Masternode

1. `scryt-cli stop` from the masternode to stop the wallet.
1. Then from your control wallet, edit your masternode.conf, delete the **masternode01** line entry.
1. Restart the control wallet.
1. Your 15,000 SCY will now be unlocked.
