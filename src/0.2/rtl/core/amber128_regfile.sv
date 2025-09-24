module amber128_regfile
  import amber128_pkg::*;
(
    input  logic                       clk_i,
    input  logic                       rst_ni,
    input  amber128_regfile_req_s      req_i,
    output logic [D_XLEN-1:0]          rd_a_o,
    output logic [D_XLEN-1:0]          rd_b_o
);

  logic [D_XLEN-1:0] regs [DATA_REG_COUNT-1:0];
  integer            i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < DATA_REG_COUNT; i++) begin
        regs[i] <= '0;
      end
    end else begin
      if (req_i.valid && req_i.we && (req_i.rw != REG_ZERO)) begin
        regs[req_i.rw] <= req_i.wd;
      end
    end
  end

  always_comb begin
    rd_a_o = (req_i.ra == REG_ZERO) ? '0 : regs[req_i.ra];
    rd_b_o = (req_i.rb == REG_ZERO) ? '0 : regs[req_i.rb];
  end

endmodule

