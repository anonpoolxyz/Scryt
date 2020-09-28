
Scryt
=====

Links
----------------

• Website: https://scrytexchange.com

• Explorer: https://explorer.scrytexchange.com

• Discord: https://discord.gg/69c5J39


For more information, as well as an immediately useable, see https://scrytexchange.com/footer/scryt.php


What is Scryt?
----------------

Scryt is a mineable SHA256 crypto currency, a coin which use the bitcoin protocol and blockchain technology.

Here are it's details and specifications:


• ASICS friendly

• 51% attacks resistant

• Adopts Segwit transations

• Based peer-to-peer cryptocurrency that is forked from Syscoin and Martkist coin

• Uses the POW (Proof of Work) combined with the Masternodes for more security and passive profits
 
 -Avoiding frozen blockchain - Blocks are fluid

• Adopts the technique of PrivateSend which allows anonymous transactions
 
 -It mixes all the transactions into multiple parts, everything inside one single transaction.


Coin Specs
----------------

<table>
<th>Name</th><th>Scryt</th>
<tr><td>Ticker</td><td>SCY</td></tr>
<tr><td>Algo</td><td>SHA 256</td></tr>
<tr><td>Block Time</td><td>60 Seconds</td></tr>
<tr><td>Block size</td><td>6 mb</td></tr>
<tr><td>Difficulty Retargeting</td><td>Every block</td></tr>
<tr><td>Max Coin Supply</td><td>50,000,000 SCY</td></tr>
<tr><td>Premine</td><td>75,000 SCY</td></tr>
<tr><td>Masternodes</td><td>60 %</td></tr>
<tr><td>Miners</td><td>50 %</td></tr>
<tr><td>P2P port</td><td>8998</td></tr>
<tr><td>RPC port</td><td>8997</td></tr>
<tr><td>POW Confirmations</td><td>100</td></tr>
<tr><td>Instant send Confirmations</td><td>6</td></tr>
<tr><td>Masternode Confirmations</td><td>13</td></tr>
</table>


Rewards 
-------

<table>
<th>Phase</th><th>Block Height</th><th>Reward</th><th>Masternodes</th><th>Miners</th><th>MN Collateral</th>
<tr><td>Phase 0</td><td>1</td><td>75000 SCY</td><td>0 SCY</td><td>100 %</td><td>500 SCY</td></tr>
<tr><td>Phase 1</td><td>2-1000</td><td>1 SCY</td><td>0.6 SCY</td><td>0.4 SCY</td><td>500 SCY</td></tr>
<tr><td>Phase 2</td><td>1001-10000</td><td>2 SCY</td><td>1.2 SCY</td><td>0.8 SCY</td><td>1000 SCY</td></tr>
<tr><td>Phase 3</td><td>10001-20000</td><td>4 SCY</td><td>2.4 SCY</td><td>1.6 SCY</td><td>1500 SCY</td></tr>
<tr><td>Phase 4</td><td>20001-40000</td><td>5 SCY</td><td>3 SCY</td><td>2 SCY</td><td>2000 SCY</td></tr>
<tr><td>Phase 5</td><td>40001-60000</td><td>6 SCY</td><td>3.6 SCY</td><td>2.4 SCY</td><td>2500 SCY</td></tr>
<tr><td>Phase 6</td><td>60001-80000</td><td>8 SCY</td><td>4.8 SCY</td><td>3.2 SCY</td><td>3000 SCY</td></tr>
<tr><td>Phase 7</td><td>80001-120000</td><td>10 SCY</td><td>6 SCY</td><td>4 SCY</td><td>3500 SCY</td></tr>
<tr><td>Phase 8</td><td>120001-150000</td><td>12 SCY</td><td>7.2 SCY</td><td>4.8 SCY</td><td>4000 SCY</td></tr>
<tr><td>Phase 9</td><td>150001-200000</td><td>14 SCY</td><td>8.4 SCY</td><td>5.6 SCY</td><td>5000 SCY</td></tr>
<tr><td>Phase 10</td><td>200001-300000</td><td>13 SCY</td><td>7.8 SCY</td><td>5.2 SCY</td><td>6000 SCY</td></tr>
<tr><td>Phase 11</td><td>300001-Collateral Lock</td><td>12 SCY</td><td>7.2 SCY</td><td>4.8 SCY</td><td>8000 SCY</td></tr>
</table>


License
-------

ScrytCoin is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is meant to be stable. Development is normally done in separate branches.
[Tags](https://github.com/scryt/scryt/tags) are created to indicate new official,
stable release versions of Scryt Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](/doc/unit-tests.md) for new code, and to
submit new unit tests for old code.

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`

Scryt test suites can run by `cd src/test && ./test_scryt`

The Travis CI system makes sure that every pull request is built for Windows
and Linux, OS X, and that unit and sanity tests are automatically run.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Scryt Core's Transifex page](https://www.transifex.com/projects/p/scryt/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.

