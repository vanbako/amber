// Amber v0.4 cluster selection package.
// Enumerates the scheduler cluster destinations used by dispatch.
package cluster_pkg;

  typedef enum logic [1:0] {
    CLUSTER_ALU        = 2'd0,
    CLUSTER_CAPABILITY = 2'd1,
    CLUSTER_LSQ        = 2'd2,
    CLUSTER_ASYNC      = 2'd3
  } cluster_sel_e;

endpackage : cluster_pkg

