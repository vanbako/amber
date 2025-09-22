module amber48_imem
  import amber48_pkg::*;
#(
    parameter string              INIT_FILE = "",
    parameter int unsigned        DEPTH     = IMEM_DEPTH
)(
    input  logic                  clk_i,
    input  logic                  rst_ni,
    input  logic [XLEN-1:0]       addr_i,
    output logic [XLEN-1:0]       data_o,
    output logic                  valid_o
);

  localparam int unsigned ADDR_LSB     = $clog2(BAU_BYTES);
  localparam int unsigned INDEX_WIDTH  = $clog2(DEPTH);

  logic [XLEN-1:0]              rom     [0:DEPTH-1];
  logic [INDEX_WIDTH-1:0]       addr_index;
  logic [XLEN-1:0]              data_q;

  initial begin
    if (DEPTH < 2) begin
      $fatal(1, "amber48_imem: DEPTH must be >= 2");
    end
    if (INIT_FILE != "") begin
      $readmemh(INIT_FILE, rom);
    end
  end

  assign addr_index = addr_i[ADDR_LSB +: INDEX_WIDTH];

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      data_q  <= '0;
      valid_o <= 1'b0;
    end else begin
      data_q  <= rom[addr_index];
      valid_o <= 1'b1;
    end
  end

  assign data_o = data_q;

endmodule
