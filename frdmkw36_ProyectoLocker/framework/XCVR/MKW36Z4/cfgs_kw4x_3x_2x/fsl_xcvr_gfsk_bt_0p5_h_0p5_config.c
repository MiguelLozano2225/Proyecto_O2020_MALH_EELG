/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "fsl_xcvr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* MODE only configuration */
const xcvr_mode_config_t gfsk_bt_0p5_h_0p5_mode_config =
{
    .radio_mode = GFSK_BT_0p5_h_0p5,
    .scgc5_clock_ena_bits = SIM_SCGC5_PHYDIG_MASK | SIM_SCGC5_GEN_FSK_MASK,

    /* XCVR_MISC configs */
    .xcvr_ctrl.mask = XCVR_CTRL_XCVR_CTRL_PROTOCOL_MASK |
                      XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC_MASK |
                      XCVR_CTRL_XCVR_CTRL_DEMOD_SEL_MASK,
    .xcvr_ctrl.init = XCVR_CTRL_XCVR_CTRL_PROTOCOL(8) |
                      XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC(7) |
                      XCVR_CTRL_XCVR_CTRL_DEMOD_SEL(1),

    .phy_pre_ref0_init = RW0PS(0, 0x19) |
                         RW0PS(1, 0x19U) |
                         RW0PS(2, 0x1AU) |
                         RW0PS(3, 0x1BU) |
                         RW0PS(4, 0x1CU) |
                         RW0PS(5, 0x1CU) |
                         RW0PS(6, 0x1DU & 0x3U), /* Phase info #6 overlaps two initialization words */
    .phy_pre_ref1_init = (0x1D) >> 2 | /* Phase info #6 overlaps two initialization words - manually compute the shift*/
                         RW1PS(7, 0x1EU) |
                         RW1PS(8, 0x1EU) |
                         RW1PS(9, 0x1EU) |
                         RW1PS(10, 0x1DU) |
                         RW1PS(11, 0x1CU) |
                         RW1PS(12, 0x1CU & 0xFU), /* Phase info #12 overlaps two initialization words */
    .phy_pre_ref2_init = (0x1C) >> 4 | /* Phase info #12 overlaps two initialization words - manually compute the shift*/
                         RW2PS(13, 0x1BU) |
                         RW2PS(14, 0x1AU) |
                         RW2PS(15, 0x19U),

    .phy_cfg1_init = XCVR_PHY_CFG1_AA_PLAYBACK(1) |
                     XCVR_PHY_CFG1_AA_OUTPUT_SEL(1) | 
                     XCVR_PHY_CFG1_FSK_BIT_INVERT(0) |
                     XCVR_PHY_CFG1_BSM_EN_BLE(0) |
                     XCVR_PHY_CFG1_DEMOD_CLK_MODE(0) |
#if RADIO_IS_GEN_2P1
                     XCVR_PHY_CFG1_CTS_THRESH(220) |
#else
                     XCVR_PHY_CFG1_CTS_THRESH(205) |
#endif /* RADIO_IS_GEN_2P1 */
                     XCVR_PHY_CFG1_FSK_FTS_TIMEOUT(2),

    .phy_el_cfg_init = XCVR_PHY_EL_CFG_EL_ENABLE(1)
#if !RADIO_IS_GEN_2P1
                     | XCVR_PHY_EL_CFG_EL_ZB_ENABLE(0)
#endif /* !RADIO_IS_GEN_2P1 */
    ,
    /* XCVR_RX_DIG configs */
    .rx_dig_ctrl_init_26mhz = XCVR_RX_DIG_RX_DIG_CTRL_RX_FSK_ZB_SEL(0) | /* Depends on protocol */
                              XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN(1) | /* Depends on protocol */
                              XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_RATE(0),

    .rx_dig_ctrl_init_32mhz = XCVR_RX_DIG_RX_DIG_CTRL_RX_FSK_ZB_SEL(0) | /* Depends on protocol */
                              XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN(1), /* Depends on protocol */

#if RADIO_IS_GEN_2P0 
    .agc_ctrl_0_init = XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0xFF),
#endif

    /* XCVR_TSM configs */
#if (DATA_PADDING_EN)
    .tsm_timing_35_init = B0(TX_DIG_EN_ASSERT+TX_DIG_EN_TX_HI_ADJ),
#else
    .tsm_timing_35_init = B0(TX_DIG_EN_ASSERT),
#endif /* (DATA_PADDING_EN) */

    /* XCVR_TX_DIG configs */
    .tx_gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_MULTIPLY_TABLE_MANUAL(0x4000) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MI(1) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MLD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MOD_INDEX_SCALING(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_OVRD_EN(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_0_OVRD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_1_OVRD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_2_OVRD(0),
    .tx_gfsk_coeff1_26mhz = 0,
    .tx_gfsk_coeff2_26mhz = 0,
    .tx_gfsk_coeff1_32mhz = 0,
    .tx_gfsk_coeff2_32mhz = 0,
};

/* MODE & DATA RATE combined configuration */

const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p5_1mbps_config =
{
    .radio_mode = GFSK_BT_0p5_h_0p5,
    .data_rate = DR_1MBPS,

    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(4) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(4), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(4), /*TZA_BW_SEL */ 

    .phy_cfg2_init = XCVR_PHY_CFG2_PHY_FIFO_PRECHG(8) |
                     XCVR_PHY_CFG2_X2_DEMOD_GAIN(0xA) ,

    /* AGC configs */
#if RADIO_IS_GEN_2P1
    .agc_ctrl_0_init_26mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xE7) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05),
    .agc_ctrl_0_init_32mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xE7) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05),               
#endif

    .agc_ctrl_2_init_26mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(10) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(1) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(2) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(10),
#endif
                             
    .agc_ctrl_2_init_32mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(12) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(1) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(2) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(10),
#endif

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(130),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(4),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(4),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(81) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(81) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif
    
    /* BLE 26MHz Channel Filter */
#if RADIO_IS_GEN_2P0
    /* All constant values are represented as 16 bits, register writes will remove unused bits */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0 = 0xFFFA,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1 = 0xFFF6,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2 = 0xFFF1,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3 = 0xFFEE,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4 = 0xFFEF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5 = 0xFFF6,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6 = 0x0004,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7 = 0x0017,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8 = 0x002F,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9 = 0x0046,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0059,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0063,

    /* BLE 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0 = 0xFFFA,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1 = 0xFFF5,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2 = 0xFFEF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3 = 0xFFEB,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4 = 0xFFEB,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5 = 0xFFF2,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6 = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7 = 0x0015,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8 = 0x0030,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9 = 0x004A,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x005F,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x006B,
#else
    /* All constant values are represented as 16 bits, register writes will remove unused bits */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0xFFFB,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0xFFF5,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF4,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFFE,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0x000A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0x001A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x002C,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x003E,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x004C,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0053,

    /* BLE 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0xFFF9,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFEB,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFEF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0x001D,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0042,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0064,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0078,
#endif /* RADIO_IS_GEN_2P0 */
 
    .rx_rccal_ctrl_0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(0) ,
    .rx_rccal_ctrl_1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(0) ,

    .tx_fsk_scale_26mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1627) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x09d9),
    .tx_fsk_scale_32mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1800) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x0800),
};

const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p5_500kbps_config =
{
    .radio_mode = GFSK_BT_0p5_h_0p5,
    .data_rate = DR_500KBPS,

    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(5) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(5), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(5), /* TZA_BW_SEL */ 

    .phy_cfg2_init = XCVR_PHY_CFG2_PHY_FIFO_PRECHG(8) |
                     XCVR_PHY_CFG2_X2_DEMOD_GAIN(0x8) ,

    /* AGC configs */
#if RADIO_IS_GEN_2P1
    .agc_ctrl_0_init_26mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05),
    .agc_ctrl_0_init_32mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x40),               
#endif

    .agc_ctrl_2_init_26mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(15) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(24),
#endif

    .agc_ctrl_2_init_32mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(18) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(24),
#endif

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(21),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(12),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(14),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif
    
    /* All constant values are represented as 16 bits, register writes will remove unused bits */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0 = 0x0004,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1 = 0x0003,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2 = 0xFFFE,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3 = 0xFFF5,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4 = 0xFFEC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5 = 0xFFE8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6 = 0xFFEE,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7 = 0x0001,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8 = 0x0020,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9 = 0x0045,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0065,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0079,
    
    /* 32MHz */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0 = 0x0005,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1 = 0x0006,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2 = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3 = 0xFFFA,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4 = 0xFFEF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5 = 0xFFE6,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6 = 0xFFE7,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7 = 0xFFF8,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8 = 0x0019,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9 = 0x0042,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0069,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0080,

    .rx_rccal_ctrl_0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(1) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(1) ,
    .rx_rccal_ctrl_1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(1) ,

    .tx_fsk_scale_26mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1627) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x09d9),
    .tx_fsk_scale_32mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1800) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x0800),
};

const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p5_250kbps_config =
{
    .radio_mode = GFSK_BT_0p5_h_0p5,
    .data_rate = DR_250KBPS,
    
    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(5) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(5), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(5), /* TZA_BW_SEL */ 

    .phy_cfg2_init = XCVR_PHY_CFG2_PHY_FIFO_PRECHG(8) |
                     XCVR_PHY_CFG2_X2_DEMOD_GAIN(0x6) ,

    /* AGC configs */                 
#if RADIO_IS_GEN_2P1
    .agc_ctrl_0_init_26mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05),
    .agc_ctrl_0_init_32mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x40),               
#endif

    .agc_ctrl_2_init_26mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(18) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) | 
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) | 
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(48),
#endif

    .agc_ctrl_2_init_32mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(22) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |                             
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(2) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) | 
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else 
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) | 
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(48),
#endif

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(21),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(15),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(18),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(70) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(70) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif
    
    /* All constant values are represented as 16 bits, register writes will remove unused bits */
#if RADIO_IS_GEN_2P0
    .rx_chf_coeffs_26mhz.rx_chf_coef_0 = 0x0002,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1 = 0xFFFD,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2 = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3 = 0xFFF1,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4 = 0xFFEC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5 = 0xFFED,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6 = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7 = 0x000B,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8 = 0x0027,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9 = 0x0046,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0060,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0070,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0 = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1 = 0xFFFD,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2 = 0xFFF8,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3 = 0xFFF1,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4 = 0xFFEC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5 = 0xFFED,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6 = 0xFFF6,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7 = 0x000A,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8 = 0x0027,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9 = 0x0046,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0061,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0071,
#else

    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0xFFFF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0xFFFC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF5,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF4,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0x0003,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0x0014,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x0029,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x003F,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0051,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x005B,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0x0005,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0x0005,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFF2,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFE8,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFEB,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0x0006,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0035,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0069,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x008B,
#endif /* RADIO_IS_GEN_2P0 */

    .rx_rccal_ctrl_0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(1) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(1) ,
    .rx_rccal_ctrl_1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(1) ,

    .tx_fsk_scale_26mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1627) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x09d9),
    .tx_fsk_scale_32mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1800) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x0800),
};

