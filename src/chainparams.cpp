// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2020 The ScrytExchange developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include "arith_uint256.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
// SCRYT includes for gen block
#include "uint256.h"
#include "arith_uint256.h"
#include "hash.h"
#include "streams.h"
#include <time.h>
// SCRYT generate block
/**
* Main network
*/
/**
* What makes a good checkpoint block?
* + Is surrounded by blocks with reasonable timestamps
*   (no blocks before with a timestamp after, none after with
*    timestamp before)
* + Contains no strange transactions
*/

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateDevNetGenesisBlock(const uint256 &prevBlockHash, const std::string& devNetName, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    assert(!devNetName.empty());

    CMutableTransaction txNew;
    txNew.nVersion = 4;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    // put height (BIP34) and devnet name into coinbase
    txNew.vin[0].scriptSig = CScript() << 1 << std::vector<unsigned char>(devNetName.begin(), devNetName.end());
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = CScript() << OP_RETURN;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock = prevBlockHash;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Crypto 03/09/2020, DeFi is the future of our finance";
    const CScript genesisOutputScript = CScript() << ParseHex("04596aa76f0244786ff2d2cbf3d76615633e3e5be301d783a349ed1e76899477b52b92ec9bb2cc9756d2b520c97952959ef02054f3790c57865bb7c09e35468336") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}
// This will figure out a valid hash and Nonce if you're
// creating a different genesis block:
static void GenerateGenesisBlock(CBlockHeader &genesisBlock, uint256 &phash)
{
	arith_uint256 bnTarget;
	bnTarget.SetCompact(genesisBlock.nBits);
	uint32_t nOnce = 0;
	while (true) {
		genesisBlock.nNonce = nOnce;
		uint256 hash = genesisBlock.GetHash();
		if (UintToArith256(hash) <= bnTarget) {
			phash = hash;
			break;
		}
		nOnce++;
	}
	printf("genesis.nTime = %u \n", genesisBlock.nTime);
	printf("genesis.nNonce = %u \n", genesisBlock.nNonce);
	printf("Generate hash = %s\n", phash.ToString().c_str());
	printf("genesis.hashMerkleRoot = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}

static CBlock FindDevNetGenesisBlock(const Consensus::Params& params, const CBlock &prevBlock, const CAmount& reward)
{
    std::string devNetName = GetDevNetName();
    assert(!devNetName.empty());

    CBlock block = CreateDevNetGenesisBlock(prevBlock.GetHash(), devNetName.c_str(), prevBlock.nTime + 1, 0, prevBlock.nBits, prevBlock.nVersion, reward);

    arith_uint256 bnTarget;
    bnTarget.SetCompact(block.nBits);

    for (uint32_t nNonce = 0; nNonce < UINT32_MAX; nNonce++) {
        block.nNonce = nNonce;

        uint256 hash = block.GetHash();
        if (UintToArith256(hash) <= bnTarget)
            return block;
    }

    // This is very unlikely to happen as we start the devnet with a very low difficulty. In many cases even the first
    // iteration of the above loop will give a result already
    error("FindDevNetGenesisBlock: could not find devnet genesis block for %s", devNetName);
    assert(false);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 0; // not used anymore
		consensus.nSeniorityInterval = 0; // not used anymore
		consensus.nTotalSeniorityIntervals = 0; // not used anymore
        consensus.nMasternodePaymentsStartBlock = 2;
        consensus.nMasternodePaymentsIncreaseBlock = 2; // actual historical value
        consensus.nMasternodePaymentsIncreasePeriod = 576*30; // 17280 - actual historical value
        consensus.nInstantSendConfirmationsRequired = 6;
        consensus.nInstantSendKeepLock = 24;
		consensus.nShareFeeBlock = 0;
        consensus.nBudgetPaymentsStartBlock = 0; // actual historical value
        consensus.nBudgetPaymentsCycleBlocks = 43800; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nSuperblockStartBlock = 2;
		consensus.nSuperblockStartHash = uint256();
        consensus.nSuperblockCycle = 1;
        consensus.nGovernanceMinQuorum = 33;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 13;
        consensus.nCollateralChangeHeightv1 = 1001;
        consensus.nCollateralChangeHeightv2 = 10001;
        consensus.nCollateralChangeHeightv3 = 20001;
        consensus.nCollateralChangeHeightv4 = 40001;
        consensus.nCollateralChangeHeightv5 = 60001;
        consensus.nCollateralChangeHeightv6 = 80001;
        consensus.nCollateralChangeHeightv7 = 120001;
        consensus.nCollateralChangeHeightv8 = 150001;
        consensus.nCollateralChangeHeightv9 = 200001;
        consensus.nCollateralChangeHeightv10 = 300001;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00000000ce346e28a61f6be67453ed2ce7da7a885c1097937ec9ff4398373dfb");
        consensus.BIP65Height = 0; // 00000000000076d8fcea02ec0963de4abfd01e771fec0863f960c2c64fe6f357
        consensus.BIP66Height = 0; // 00000000000b1fa2dfa312863570e13fae9ca7b5566cb27e55422620b469aefa
        consensus.DIP0001Height = 0;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20
        consensus.nPowTargetTimespan = 1 * 60;// 00:01:00
        consensus.nPowTargetSpacing = 1 * 60;// 00:01:00
		consensus.nAuxpowChainId = 0xfff;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 15200;
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL; // Feb 5th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0; // Oct 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL; // Oct 15th, 2018

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0; // Apr 23th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL; // Apr 23th, 2019


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000100010001");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000ce346e28a61f6be67453ed2ce7da7a885c1097937ec9ff4398373dfb"); // 0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
		pchMessageStart[0] = 0xa4;
		pchMessageStart[1] = 0xd6;
		pchMessageStart[2] = 0xb4;
		pchMessageStart[3] = 0xc9;
       		vAlertPubKey = ParseHex("04596aa76f0244786ff2d2cbf3d76615633e3e5be301d783a349ed1e76899477b52b92ec9bb2cc9756d2b520c97952959ef02054f3790c57865bb7c09e35468336");
		nDefaultPort = 8998;
		nPruneAfterHeight = 1000000;
		uint256 hash;
		genesis = CreateGenesisBlock(1599229138, 545477356, 0x1d00ffff, 1, 100 * COIN);
		/*CBlockHeader genesisHeader = genesis.GetBlockHeader();
		GenerateGenesisBlock(genesisHeader, &hash);*/

		consensus.hashGenesisBlock = genesis.GetHash();
		assert(consensus.hashGenesisBlock == uint256S("00000000ce346e28a61f6be67453ed2ce7da7a885c1097937ec9ff4398373dfb"));
		assert(genesis.hashMerkleRoot == uint256S("48621c2f7fe2a402ff7c6b556f5177d16afb76a3330c2d052854ae659dc8bc43"));

		vSeeds.push_back(CDNSSeedData("104.238.190.249", "104.238.190.249"));
		vSeeds.push_back(CDNSSeedData("95.179.210.253", "95.179.210.253"));

		base58Prefixes[PUBKEY_ADDRESS_SCY] = std::vector<unsigned char>(1, 63);
		base58Prefixes[SCRIPT_ADDRESS_SCY] = std::vector<unsigned char>(1, 125);
		base58Prefixes[SECRET_KEY_SCY] = std::vector<unsigned char>(1, 48);

		base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 0);
		base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 5);
		base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 128);

		base58Prefixes[PUBKEY_ADDRESS_ZEC] = { 0x1C,0xB8 };
		base58Prefixes[SCRIPT_ADDRESS_ZEC] = { 0x1C,0xBD };
		base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 128);
		// Scryt BIP32 pubkeys
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xb2)(0x1e).convert_to_container<std::vector<unsigned char> >();
		// Scryt BIP32 prvkeys
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xad)(0xe4).convert_to_container<std::vector<unsigned char> >();

        // Scryt BIP44 coin type is '0x800004d2'
        nExtCoinType = 0x800004d2;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = false;
        fAllowMultiplePorts = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5 * 60;// 5min

        strSporkAddress = "SgEJ5XcvFCiHnYF3qAphz52nGknkdyqKSF";
		

        checkpointData = {
            {
                { 0, uint256S("00000000ce346e28a61f6be67453ed2ce7da7a885c1097937ec9ff4398373dfb")},
            }
        };
	    
		chainTxData = ChainTxData{
			// Data from rpc: getchaintxstats 0 00000000ce346e28a61f6be67453ed2ce7da7a885c1097937ec9ff4398373dfb
			/* nTime    */ 0,
			/* nTxCount */ 0,
			/* dTxRate  */ 0
		};
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 0; // not used anymore
		consensus.nSeniorityInterval = 0; // not used anymore
		consensus.nTotalSeniorityIntervals = 0; // not used anymore
        consensus.nMasternodePaymentsStartBlock = 2;
        consensus.nMasternodePaymentsIncreaseBlock = 2;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 2;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 0;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
		consensus.nShareFeeBlock = 0;
        consensus.nSuperblockStartBlock = 0;
        consensus.nSuperblockStartHash = uint256();
        consensus.nSuperblockCycle = 1;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 2;
        consensus.nCollateralChangeHeightv1 = 1001;
        consensus.nCollateralChangeHeightv2 = 10001;
        consensus.nCollateralChangeHeightv3 = 20001;
        consensus.nCollateralChangeHeightv4 = 40001;
        consensus.nCollateralChangeHeightv5 = 60001;
        consensus.nCollateralChangeHeightv6 = 80001;
        consensus.nCollateralChangeHeightv7 = 120001;
        consensus.nCollateralChangeHeightv8 = 150001;
        consensus.nCollateralChangeHeightv9 = 200001;
        consensus.nCollateralChangeHeightv10 = 300001;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; // 0000039cf01242c7f921dcb4806a5994bc003b48c1973ae0c89b67809c2bb2ab
        consensus.BIP66Height = 0; // 0000002acdd29a14583540cb72e1c5cc83783560e38fa7081495d474fe1671f7
        consensus.DIP0001Height = 5500;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20
		consensus.nPowTargetTimespan = 1 * 60;// 00h 01min 00s
		consensus.nPowTargetSpacing = 1 * 60;// 00h 01min 00s
		consensus.nAuxpowChainId = 0xfff;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL; // Sep 18th, 2018

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0; // Feb 5th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL; // Feb 5th, 2019


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0xd0;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xc2;
        vAlertPubKey = ParseHex("04596aa76f0244786ff2d2cbf3d76615633e3e5be301d783a349ed1e76899477b52b92ec9bb2cc9756d2b520c97952959ef02054f3790c57865bb7c09e35468336"); // e91867736fe8087ac7d5ac0d9974b0340978eb8a1a7f0771b1d22682b68d0406
        nDefaultPort = 18998;
        nPruneAfterHeight = 1000;

		genesis = CreateGenesisBlock(1599229138, 545477356, 0x1d00ffff, 1, 100 * COIN);
		/*
		uint256 hash;
		CBlockHeader genesisHeader = genesis.GetBlockHeader();
		GenerateGenesisBlock(genesisHeader, &hash);*/
		consensus.hashGenesisBlock = genesis.GetHash();
		assert(consensus.hashGenesisBlock == uint256S("00000000ce346e28a61f6be67453ed2ce7da7a885c1097937ec9ff4398373dfb"));
		assert(genesis.hashMerkleRoot == uint256S("48621c2f7fe2a402ff7c6b556f5177d16afb76a3330c2d052854ae659dc8bc43"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
        vSeeds.clear();
  

		base58Prefixes[PUBKEY_ADDRESS_SCY] = std::vector<unsigned char>(1, 65);
		base58Prefixes[SCRIPT_ADDRESS_SCY] = std::vector<unsigned char>(1, 60);
		base58Prefixes[SECRET_KEY_SCY] = std::vector<unsigned char>(1, 23);

		base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 111);
		base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 196);
		base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 239);

		base58Prefixes[PUBKEY_ADDRESS_ZEC] = std::vector<unsigned char>(0x1C, 0xB8);
		base58Prefixes[SCRIPT_ADDRESS_ZEC] = std::vector<unsigned char>(0x1C, 0xBD);
		base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 239);

        // Testnet Scryt BIP44 coin type is '0x800004d3'
        nExtCoinType = 0x800004d3;


        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = false;
        fAllowMultiplePorts = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5 * 60;// 5min

        strSporkAddress = "TPzPSch2Kw1EwJua9joTqf7mwTvuLXpRbC"; // 4Tn2CyZ7GABqT4DTDooN8SZuqRpoiBonRJY1Zf68WtUzUEwkZ9GJ

	

		chainTxData = ChainTxData{
			0,
			0,
			0
		};

		

    }
};
static CTestNetParams testNetParams;

/**
 * Devnet
 */
class CDevNetParams : public CChainParams {
public:
    CDevNetParams() {
        strNetworkID = "dev";
        consensus.nSubsidyHalvingInterval = 0; // not used anymore
        consensus.nMasternodePaymentsStartBlock = 2;
        consensus.nMasternodePaymentsIncreaseBlock = 4030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 6;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nSuperblockStartBlock = 0;
        consensus.nSuperblockStartHash = uint256(); // do not check this on devnet
        consensus.nSuperblockCycle = 49000;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 2; // BIP34 activated immediately on devnet
        consensus.BIP65Height = 2; // BIP65 activated immediately on devnet
        consensus.BIP66Height = 2; // BIP66 activated immediately on devnet
        consensus.DIP0001Height = 2; // DIP0001 activated immediately on devnet
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1
        consensus.nPowTargetTimespan = 1 * 60;// 00:01:00
        consensus.nPowTargetSpacing = 1 * 60;// 00:01:00
		consensus.nAuxpowChainId = 0xfff;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL; // Sep 18th, 2018

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0; // Feb 5th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL; // Feb 5th, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xe9;
        pchMessageStart[1] = 0xd3;
        pchMessageStart[2] = 0xf1;
        pchMessageStart[3] = 0xb5;
        vAlertPubKey = ParseHex("04596aa76f0244786ff2d2cbf3d76615633e3e5be301d783a349ed1e76899477b52b92ec9bb2cc9756d2b520c97952959ef02054f3790c57865bb7c09e35468336"); // 3c464804a17d2dab8aeb57a8a8c72463ba73e884fe11590c194e03d105311035
        nDefaultPort = 28998;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1599229138, 545477356, 0x1d00ffff, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("00000000ce346e28a61f6be67453ed2ce7da7a885c1097937ec9ff4398373dfb"));
        assert(genesis.hashMerkleRoot == uint256S("48621c2f7fe2a402ff7c6b556f5177d16afb76a3330c2d052854ae659dc8bc43"));

        devnetGenesis = FindDevNetGenesisBlock(consensus, genesis, 1 * COIN);
        consensus.hashDevnetGenesisBlock = devnetGenesis.GetHash();

        vFixedSeeds.clear();
        vSeeds.clear();
        

		base58Prefixes[PUBKEY_ADDRESS_SCY] = std::vector<unsigned char>(1, 66);
		base58Prefixes[SCRIPT_ADDRESS_SCY] = std::vector<unsigned char>(1, 61);
		base58Prefixes[SECRET_KEY_SCY] = std::vector<unsigned char>(1, 24);

		base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 111);
		base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 196);
		base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 239);

		base58Prefixes[PUBKEY_ADDRESS_ZEC] = { 0x1C,0xB8 };
		base58Prefixes[SCRIPT_ADDRESS_ZEC] = { 0x1C,0xBD };
		base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 239);
		// Regtest Scryt BIP32 pubkeys
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xcf).convert_to_container<std::vector<unsigned char> >();
		// Regtest Scryt BIP32 prvkeys
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Testnet Scryt BIP44 coin type is '0x800004d4'
        nExtCoinType = 0x800004d4;

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = true;
        fAllowMultiplePorts = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5 * 60;// 5min

        strSporkAddress = "Th5rogePVBXL4FGdTixpupbSKgthujGguh"; // 4gLGkvSyBQXh6Ubd465MB7RVwLBpLScC66AKh6XTFV2eCP5KyAdh

		

		chainTxData = ChainTxData{
			0,
			0,
			0
		};
    }
};
static CDevNetParams *devNetParams;


/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
		strNetworkID = "regtest";
		consensus.nSeniorityInterval = 0; // not used anymore
		consensus.nTotalSeniorityIntervals = 0; // not used anymore

        consensus.nSubsidyHalvingInterval = 0; // not used anymore
        consensus.nMasternodePaymentsStartBlock = 2;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 6;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 0;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nSuperblockStartBlock = 0;
        consensus.nSuperblockStartHash = uint256(); // do not check this on regtest
        consensus.nSuperblockCycle = 49000;
		consensus.nShareFeeBlock = 0;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 0; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.DIP0001Height = 2000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1
        consensus.nPowTargetTimespan = 1 * 60;// 00:01:00
        consensus.nPowTargetSpacing = 1 * 60;// 00:01:00
		consensus.nAuxpowChainId = 0xfff;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 15200; // same as mainnet
        consensus.nPowDGWHeight = 0; // same as mainnet
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

		pchMessageStart[0] = 0xf2;
		pchMessageStart[1] = 0xb0;
		pchMessageStart[2] = 0xb8;
		pchMessageStart[3] = 0xd3;
        nDefaultPort = 38998;
        nPruneAfterHeight = 1000;

		genesis = CreateGenesisBlock(1599229138, 545477356, 0x1d00ffff, 1, 100 * COIN);
		/*
		uint256 hash;
		CBlockHeader genesisHeader = genesis.GetBlockHeader();
		GenerateGenesisBlock(genesisHeader, &hash);*/
		consensus.hashGenesisBlock = genesis.GetHash();
		// assert(consensus.hashGenesisBlock == uint256S(""));
		// assert(genesis.hashMerkleRoot == uint256S(""));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;
        fAllowMultipleAddressesFromGroup = true;
        fAllowMultiplePorts = true;

        nFulfilledRequestExpireTime = 5 * 60;// 5min

        
        strSporkAddress = "TrFBsyeRFRDWiPZFHyiyrSJg1S49EVJtQy"; // 4aF2Q9pMjw6wYXH8wsEVkniDBSh2n4nV4woVodAMYaN4Cmf18fpm

   

		base58Prefixes[PUBKEY_ADDRESS_SCY] = std::vector<unsigned char>(1, 66);
		base58Prefixes[SCRIPT_ADDRESS_SCY] = std::vector<unsigned char>(1, 61);
		base58Prefixes[SECRET_KEY_SCY] = std::vector<unsigned char>(1, 24);

		base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 111);
		base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 196);
		base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 239);

		base58Prefixes[PUBKEY_ADDRESS_ZEC] = { 0x1C,0xB8 };
		base58Prefixes[SCRIPT_ADDRESS_ZEC] = { 0x1C,0xBD };
		base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 239);
		// Regtest Scryt BIP32 pubkeys
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xcf).convert_to_container<std::vector<unsigned char> >();
		// Regtest Scryt BIP32 prvkeys
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest Scryt BIP44 coin type is '0x800004d5'
        nExtCoinType = 0x800004d5;
		

		chainTxData = ChainTxData{
			0,
			0,
			0
		};
   }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::DEVNET) {
            assert(devNetParams);
            return *devNetParams;
    } else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    if (network == CBaseChainParams::DEVNET) {
        devNetParams = new CDevNetParams();
    }

    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
