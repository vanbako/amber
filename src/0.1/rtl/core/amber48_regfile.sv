module amber48_regfile
  import amber48_pkg::*;
(
    input  logic                   clk_i,
    input  logic                   rst_ni,
    input  amber48_regfile_req_s   req_i,
    output logic [XLEN-1:0]        rd_a_o,
    output logic [XLEN-1:0]        rd_b_o
);

  logic [XLEN-1:0] regs [REG_COUNT-1:0];
  integer          idx;
  localparam logic [REG_ADDR_WIDTH-1:0] REG_ZERO = '0;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (idx = 0; idx < REG_COUNT; idx++) begin
        regs[idx] <= '0;
      end
    end else begin
      if (req_i.we && (req_i.addr_w != REG_ZERO)) begin
        regs[req_i.addr_w] <= req_i.wd;
      end
    end
  end

  always_comb begin
    rd_a_o = (req_i.addr_a == REG_ZERO) ? '0 : regs[req_i.addr_a];
    rd_b_o = (req_i.addr_b == REG_ZERO) ? '0 : regs[req_i.addr_b];
  end

endmodule
