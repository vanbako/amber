`timescale 1ns/1ps

module amber128_reset_sync
#(
    parameter int unsigned STAGES = 2
)(
    input  logic clk_i,
    input  logic rst_ni,
    output logic rst_no
);

  logic [STAGES-1:0] sync_ff;

  initial begin
    if (STAGES < 2) begin
      $fatal(1, "amber128_reset_sync: STAGES must be >= 2");
    end
    sync_ff = '0;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      sync_ff <= '0;
    end else begin
      sync_ff <= {sync_ff[STAGES-2:0], 1'b1};
    end
  end

  assign rst_no = sync_ff[STAGES-1];

endmodule