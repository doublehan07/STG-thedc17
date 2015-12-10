LIBRARY IEEE;
USE IEEE.std_logic_1164.all;

ENTITY SCHEMATIC1 IS 

END SCHEMATIC1;



ARCHITECTURE STRUCTURE OF SCHEMATIC1 IS

-- COMPONENTS

COMPONENT STM32F429VG
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\5\ : INOUT std_logic;
	VBAT : IN std_logic;
	\7\ : INOUT std_logic;
	\8\ : INOUT std_logic;
	\9\ : INOUT std_logic;
	VSS : IN std_logic;
	VDD : IN std_logic;
	\12\ : INOUT std_logic;
	\13\ : INOUT std_logic;
	NRST : INOUT std_logic;
	\15\ : INOUT std_logic;
	\16\ : INOUT std_logic;
	\17\ : INOUT std_logic;
	\18\ : INOUT std_logic;
	VSSA : IN std_logic;
	\VREF+\ : IN std_logic;
	VDDA : IN std_logic;
	\23\ : INOUT std_logic;
	\24\ : INOUT std_logic;
	\25\ : INOUT std_logic;
	\26\ : INOUT std_logic;
	\29\ : INOUT std_logic;
	\30\ : INOUT std_logic;
	\31\ : INOUT std_logic;
	\32\ : INOUT std_logic;
	\33\ : INOUT std_logic;
	\34\ : INOUT std_logic;
	\35\ : INOUT std_logic;
	\36\ : INOUT std_logic;
	PB2_BOOT1 : INOUT std_logic;
	\38\ : INOUT std_logic;
	\39\ : INOUT std_logic;
	\40\ : INOUT std_logic;
	\41\ : INOUT std_logic;
	\42\ : INOUT std_logic;
	\43\ : INOUT std_logic;
	\44\ : INOUT std_logic;
	\45\ : INOUT std_logic;
	\46\ : INOUT std_logic;
	\47\ : INOUT std_logic;
	\48\ : INOUT std_logic;
	VCAP_1 : IN std_logic;
	\51\ : INOUT std_logic;
	\52\ : INOUT std_logic;
	\53\ : INOUT std_logic;
	\54\ : INOUT std_logic;
	\55\ : INOUT std_logic;
	\56\ : INOUT std_logic;
	\57\ : INOUT std_logic;
	\58\ : INOUT std_logic;
	\59\ : INOUT std_logic;
	\60\ : INOUT std_logic;
	\61\ : INOUT std_logic;
	\62\ : INOUT std_logic;
	\63\ : INOUT std_logic;
	\64\ : INOUT std_logic;
	\65\ : INOUT std_logic;
	\66\ : INOUT std_logic;
	\67\ : INOUT std_logic;
	\68\ : INOUT std_logic;
	\69\ : INOUT std_logic;
	\70\ : INOUT std_logic;
	\71\ : INOUT std_logic;
	\72\ : INOUT std_logic;
	VCAP_2 : IN std_logic;
	\76\ : INOUT std_logic;
	\77\ : INOUT std_logic;
	\78\ : INOUT std_logic;
	\79\ : INOUT std_logic;
	\80\ : INOUT std_logic;
	\81\ : INOUT std_logic;
	\82\ : INOUT std_logic;
	\83\ : INOUT std_logic;
	\84\ : INOUT std_logic;
	\85\ : INOUT std_logic;
	\86\ : INOUT std_logic;
	\87\ : INOUT std_logic;
	\88\ : INOUT std_logic;
	\89\ : INOUT std_logic;
	\90\ : INOUT std_logic;
	\91\ : INOUT std_logic;
	\92\ : INOUT std_logic;
	\93\ : INOUT std_logic;
	BOOT0 : INOUT std_logic;
	\95\ : INOUT std_logic;
	\96\ : INOUT std_logic;
	\97\ : INOUT std_logic;
	\98\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \LM2596-3.3\
	PORT (
	VIN : INOUT std_logic;
	\OUT\ : INOUT std_logic;
	GND : INOUT std_logic;
	FB : INOUT std_logic;
	\ON\ : INOUT std_logic;
	TGND : INOUT std_logic
	); END COMPONENT;

COMPONENT \0\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \33u\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \FUSE 2A\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \104\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \8M\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT SWD
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic
	); END COMPONENT;

COMPONENT LED
	PORT (
	ANODE : INOUT std_logic;
	CATHODE : INOUT std_logic
	); END COMPONENT;

COMPONENT BATT
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT MPU9250
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \1N5824\
	PORT (
	A : INOUT std_logic;
	K : INOUT std_logic
	); END COMPONENT;

COMPONENT \220u 25V\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

-- SIGNALS

SIGNAL \0\ : std_logic;
SIGNAL \11V1\ : std_logic;
SIGNAL \3V3\ : std_logic;
SIGNAL \5V\ : std_logic;
SIGNAL ADC0 : std_logic;
SIGNAL ADC1 : std_logic;
SIGNAL ADC10 : std_logic;
SIGNAL ADC11 : std_logic;
SIGNAL ADC12 : std_logic;
SIGNAL ADC13 : std_logic;
SIGNAL ADC2 : std_logic;
SIGNAL ADC3 : std_logic;
SIGNAL I2C1_SCL : std_logic;
SIGNAL I2C1_SDA : std_logic;
SIGNAL I2C2_SCL : std_logic;
SIGNAL I2C2_SDA : std_logic;
SIGNAL N01350 : std_logic;
SIGNAL N01406 : std_logic;
SIGNAL N02191 : std_logic;
SIGNAL N02195 : std_logic;
SIGNAL N04292 : std_logic;
SIGNAL N05431 : std_logic;
SIGNAL N05435 : std_logic;
SIGNAL N06205 : std_logic;
SIGNAL N06618 : std_logic;
SIGNAL N06726 : std_logic;
SIGNAL N069841 : std_logic;
SIGNAL N070001 : std_logic;
SIGNAL N070161 : std_logic;
SIGNAL N070321 : std_logic;
SIGNAL N070481 : std_logic;
SIGNAL N070641 : std_logic;
SIGNAL N070801 : std_logic;
SIGNAL N070961 : std_logic;
SIGNAL \OFFPAGELEFT-R\ : std_logic;
SIGNAL PD10 : std_logic;
SIGNAL PD11 : std_logic;
SIGNAL PD12 : std_logic;
SIGNAL PD13 : std_logic;
SIGNAL PD14 : std_logic;
SIGNAL PD15 : std_logic;
SIGNAL PD8 : std_logic;
SIGNAL PD9 : std_logic;
SIGNAL PE12 : std_logic;
SIGNAL PE13 : std_logic;
SIGNAL PE14 : std_logic;
SIGNAL PE15 : std_logic;
SIGNAL SPI2_MISO : std_logic;
SIGNAL SPI2_MOSI : std_logic;
SIGNAL SPI2_SCK : std_logic;
SIGNAL SPI4_MISO : std_logic;
SIGNAL SPI4_MOSI : std_logic;
SIGNAL SPI4_SCK : std_logic;
SIGNAL SWCLK : std_logic;
SIGNAL SWDIO : std_logic;
SIGNAL TIM1_CH1 : std_logic;
SIGNAL TIM1_CH1N : std_logic;
SIGNAL TIM1_CH2 : std_logic;
SIGNAL TIM1_CH2N : std_logic;
SIGNAL TIM8_CH1 : std_logic;
SIGNAL TIM8_CH1N : std_logic;
SIGNAL TIM8_CH2 : std_logic;
SIGNAL TIM8_CH2N : std_logic;
SIGNAL UART5_RX : std_logic;
SIGNAL UART5_TX : std_logic;
SIGNAL UART8_RX : std_logic;
SIGNAL UART8_TX : std_logic;
SIGNAL USART1_RX : std_logic;
SIGNAL USART1_TX : std_logic;
SIGNAL USART2_RX : std_logic;
SIGNAL USART2_TX : std_logic;

-- INSTANCE ATTRIBUTES



-- GATE INSTANCES

BEGIN
U1 : STM32F429VG	PORT MAP(
	\1\ => SPI4_SCK, 
	\2\ => OPEN, 
	\3\ => OPEN, 
	\4\ => SPI4_MISO, 
	\5\ => SPI4_MOSI, 
	VBAT => \3V3\, 
	\7\ => OPEN, 
	\8\ => OPEN, 
	\9\ => OPEN, 
	VSS => \0\, 
	VDD => \3V3\, 
	\12\ => N01350, 
	\13\ => N01406, 
	NRST => OPEN, 
	\15\ => ADC10, 
	\16\ => ADC11, 
	\17\ => ADC12, 
	\18\ => ADC13, 
	VSSA => \0\, 
	\VREF+\ => \3V3\, 
	VDDA => \3V3\, 
	\23\ => ADC0, 
	\24\ => ADC1, 
	\25\ => ADC2, 
	\26\ => ADC3, 
	\29\ => OPEN, 
	\30\ => OPEN, 
	\31\ => OPEN, 
	\32\ => TIM8_CH1N, 
	\33\ => OPEN, 
	\34\ => OPEN, 
	\35\ => TIM8_CH2N, 
	\36\ => OPEN, 
	PB2_BOOT1 => OPEN, 
	\38\ => OPEN, 
	\39\ => TIM1_CH1N, 
	\40\ => TIM1_CH1, 
	\41\ => TIM1_CH2N, 
	\42\ => TIM1_CH2, 
	\43\ => PE12, 
	\44\ => PE13, 
	\45\ => PE14, 
	\46\ => PE15, 
	\47\ => I2C2_SCL, 
	\48\ => I2C2_SDA, 
	VCAP_1 => N02195, 
	\51\ => OPEN, 
	\52\ => SPI2_SCK, 
	\53\ => SPI2_MISO, 
	\54\ => SPI2_MOSI, 
	\55\ => PD8, 
	\56\ => PD9, 
	\57\ => PD10, 
	\58\ => PD11, 
	\59\ => PD12, 
	\60\ => PD13, 
	\61\ => PD14, 
	\62\ => PD15, 
	\63\ => TIM8_CH1, 
	\64\ => TIM8_CH2, 
	\65\ => OPEN, 
	\66\ => OPEN, 
	\67\ => OPEN, 
	\68\ => USART1_TX, 
	\69\ => USART1_RX, 
	\70\ => OPEN, 
	\71\ => OPEN, 
	\72\ => SWDIO, 
	VCAP_2 => N02191, 
	\76\ => SWCLK, 
	\77\ => OPEN, 
	\78\ => OPEN, 
	\79\ => OPEN, 
	\80\ => UART5_TX, 
	\81\ => OPEN, 
	\82\ => OPEN, 
	\83\ => UART5_RX, 
	\84\ => OPEN, 
	\85\ => OPEN, 
	\86\ => USART2_TX, 
	\87\ => USART2_RX, 
	\88\ => OPEN, 
	\89\ => OPEN, 
	\90\ => OPEN, 
	\91\ => OPEN, 
	\92\ => I2C1_SCL, 
	\93\ => I2C1_SDA, 
	BOOT0 => N04292, 
	\95\ => OPEN, 
	\96\ => OPEN, 
	\97\ => UART8_RX, 
	\98\ => UART8_TX
);
U2 : \LM2596-3.3\	PORT MAP(
	VIN => N06205, 
	\OUT\ => N06618, 
	GND => \0\, 
	FB => \3V3\, 
	\ON\ => \0\, 
	TGND => \0\
);
U3 : \LM2596-5.0\	PORT MAP(
	VIN => N06205, 
	\OUT\ => N06726, 
	GND => \0\, 
	FB => \5V\, 
	\ON\ => \0\, 
	TGND => \0\
);
R1 : \0\	PORT MAP(
	\1\ => \0\, 
	\2\ => N04292
);
R2 : \100\	PORT MAP(
	\1\ => SWDIO, 
	\2\ => N05431
);
R3 : \100\	PORT MAP(
	\1\ => SWCLK, 
	\2\ => N05435
);
R4 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N069841
);
R5 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N070001
);
L1 : \33u\	PORT MAP(
	\1\ => N06618, 
	\2\ => \3V3\
);
R6 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N070161
);
L2 : \33u\	PORT MAP(
	\1\ => N06726, 
	\2\ => \5V\
);
R7 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N070321
);
R8 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N070481
);
F1 : \FUSE 2A\	PORT MAP(
	\1\ => N06205, 
	\2\ => \11V1\
);
R9 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N070641
);
C1 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C2 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C3 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C4 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C5 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C6 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C7 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C8 : \104\	PORT MAP(
	\1\ => N02195, 
	\2\ => \0\
);
C9 : \104\	PORT MAP(
	\1\ => N02191, 
	\2\ => \0\
);
Y1 : \8M\	PORT MAP(
	\1\ => N01350, 
	\2\ => N01406
);
R10 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N070801
);
R11 : \100\	PORT MAP(
	\1\ => \OFFPAGELEFT-R\, 
	\2\ => N070961
);
J1 : SWD	PORT MAP(
	\1\ => N05431, 
	\2\ => N05435, 
	\3\ => \0\
);
D10 : LED	PORT MAP(
	ANODE => N070961, 
	CATHODE => \0\
);
J2 : BATT	PORT MAP(
	\1\ => N06205, 
	\2\ => \0\
);
J3 : MPU9250	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\, 
	\3\ => I2C2_SCL, 
	\4\ => I2C2_SDA
);
D1 : \1N5824\	PORT MAP(
	A => \0\, 
	K => N06618
);
D2 : \1N5824\	PORT MAP(
	A => \0\, 
	K => N06726
);
D3 : LED	PORT MAP(
	ANODE => N069841, 
	CATHODE => \0\
);
D4 : LED	PORT MAP(
	ANODE => N070001, 
	CATHODE => \0\
);
D5 : LED	PORT MAP(
	ANODE => N070161, 
	CATHODE => \0\
);
D6 : LED	PORT MAP(
	ANODE => N070321, 
	CATHODE => \0\
);
D7 : LED	PORT MAP(
	ANODE => N070481, 
	CATHODE => \0\
);
D8 : LED	PORT MAP(
	ANODE => N070641, 
	CATHODE => \0\
);
D9 : LED	PORT MAP(
	ANODE => N070801, 
	CATHODE => \0\
);
C10 : \20p\	PORT MAP(
	\1\ => N01350, 
	\2\ => \0\
);
C11 : \20p\	PORT MAP(
	\1\ => N01406, 
	\2\ => \0\
);
C12 : \220u 25V\	PORT MAP(
	\1\ => N06205, 
	\2\ => \0\
);
C13 : \220u\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
C14 : \220u\	PORT MAP(
	\1\ => \5V\, 
	\2\ => \0\
);
C15 : \104\	PORT MAP(
	\1\ => \3V3\, 
	\2\ => \0\
);
END STRUCTURE;

