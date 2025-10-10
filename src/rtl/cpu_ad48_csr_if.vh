`ifndef CPU_AD48_CSR_IF_VH
`define CPU_AD48_CSR_IF_VH

// Packed bus definitions for the csr_unit request/response interface.
localparam integer CSR_REQ_FUNC_WIDTH  = 4;
localparam integer CSR_REQ_ADDR_WIDTH  = 12;
localparam integer CSR_REQ_IDX_WIDTH   = 3;
localparam integer CSR_REQ_PRIV_WIDTH  = 2;
localparam integer CSR_REQ_DATA_WIDTH  = 48;

localparam integer CSR_REQ_VALID_BIT   = 0;
localparam integer CSR_REQ_FUNC_LSB    = CSR_REQ_VALID_BIT + 1;
localparam integer CSR_REQ_FUNC_MSB    = CSR_REQ_FUNC_LSB + CSR_REQ_FUNC_WIDTH - 1;
localparam integer CSR_REQ_ADDR_LSB    = CSR_REQ_FUNC_MSB + 1;
localparam integer CSR_REQ_ADDR_MSB    = CSR_REQ_ADDR_LSB + CSR_REQ_ADDR_WIDTH - 1;
localparam integer CSR_REQ_RD_IS_A_BIT = CSR_REQ_ADDR_MSB + 1;
localparam integer CSR_REQ_RD_IDX_LSB  = CSR_REQ_RD_IS_A_BIT + 1;
localparam integer CSR_REQ_RD_IDX_MSB  = CSR_REQ_RD_IDX_LSB + CSR_REQ_IDX_WIDTH - 1;
localparam integer CSR_REQ_SRC_LSB     = CSR_REQ_RD_IDX_MSB + 1;
localparam integer CSR_REQ_SRC_MSB     = CSR_REQ_SRC_LSB + CSR_REQ_DATA_WIDTH - 1;
localparam integer CSR_REQ_PRIV_LSB    = CSR_REQ_SRC_MSB + 1;
localparam integer CSR_REQ_PRIV_MSB    = CSR_REQ_PRIV_LSB + CSR_REQ_PRIV_WIDTH - 1;
localparam integer CSR_REQ_WIDTH       = CSR_REQ_PRIV_MSB + 1;

localparam integer CSR_RESP_DATA_WIDTH   = 48;
localparam integer CSR_RESP_OK_BIT       = 0;
localparam integer CSR_RESP_ILLEGAL_BIT  = CSR_RESP_OK_BIT + 1;
localparam integer CSR_RESP_WRITE_EN_BIT = CSR_RESP_ILLEGAL_BIT + 1;
localparam integer CSR_RESP_READ_LSB     = CSR_RESP_WRITE_EN_BIT + 1;
localparam integer CSR_RESP_READ_MSB     = CSR_RESP_READ_LSB + CSR_RESP_DATA_WIDTH - 1;
localparam integer CSR_RESP_WRITE_LSB    = CSR_RESP_READ_MSB + 1;
localparam integer CSR_RESP_WRITE_MSB    = CSR_RESP_WRITE_LSB + CSR_RESP_DATA_WIDTH - 1;
localparam integer CSR_RESP_WIDTH        = CSR_RESP_WRITE_MSB + 1;

localparam integer CSR_WB_DATA_WIDTH = 48;
localparam integer CSR_WB_VALID_BIT  = 0;
localparam integer CSR_WB_IS_A_BIT   = CSR_WB_VALID_BIT + 1;
localparam integer CSR_WB_IDX_LSB    = CSR_WB_IS_A_BIT + 1;
localparam integer CSR_WB_IDX_MSB    = CSR_WB_IDX_LSB + CSR_REQ_IDX_WIDTH - 1;
localparam integer CSR_WB_DATA_LSB   = CSR_WB_IDX_MSB + 1;
localparam integer CSR_WB_DATA_MSB   = CSR_WB_DATA_LSB + CSR_WB_DATA_WIDTH - 1;
localparam integer CSR_WB_WIDTH      = CSR_WB_DATA_MSB + 1;

`endif // CPU_AD48_CSR_IF_VH
