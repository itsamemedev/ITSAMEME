# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libitsameme_cli*         | RPC client functionality used by *itsameme-cli* executable |
| *libitsameme_common*      | Home for common functionality shared by different executables and libraries. Similar to *libitsameme_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libitsameme_consensus*   | Stable, backwards-compatible consensus functionality used by *libitsameme_node* and *libitsameme_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libitsamemeconsensus*    | Shared library build of static *libitsameme_consensus* library |
| *libitsameme_kernel*      | Consensus engine and support library used for validation by *libitsameme_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libitsamemeqt*           | GUI functionality used by *itsameme-qt* and *itsameme-gui* executables |
| *libitsameme_ipc*         | IPC functionality used by *itsameme-node*, *itsameme-wallet*, *itsameme-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libitsameme_node*        | P2P and RPC server functionality used by *itsamemed* and *itsameme-qt* executables. |
| *libitsameme_util*        | Home for common functionality shared by different executables and libraries. Similar to *libitsameme_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libitsameme_wallet*      | Wallet functionality used by *itsamemed* and *itsameme-wallet* executables. |
| *libitsameme_wallet_tool* | Lower-level wallet functionality used by *itsameme-wallet* executable. |
| *libitsameme_zmq*         | [ZeroMQ](../zmq.md) functionality used by *itsamemed* and *itsameme-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libitsameme_consensus* and *libitsameme_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libitsameme_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libitsameme_node* code lives in `src/node/` in the `node::` namespace
  - *libitsameme_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libitsameme_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libitsameme_util* code lives in `src/util/` in the `util::` namespace
  - *libitsameme_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

itsameme-cli[itsameme-cli]-->libitsameme_cli;

itsamemed[itsamemed]-->libitsameme_node;
itsamemed[itsamemed]-->libitsameme_wallet;

itsameme-qt[itsameme-qt]-->libitsameme_node;
itsameme-qt[itsameme-qt]-->libitsamemeqt;
itsameme-qt[itsameme-qt]-->libitsameme_wallet;

itsameme-wallet[itsameme-wallet]-->libitsameme_wallet;
itsameme-wallet[itsameme-wallet]-->libitsameme_wallet_tool;

libitsameme_cli-->libitsameme_util;
libitsameme_cli-->libitsameme_common;

libitsameme_common-->libitsameme_consensus;
libitsameme_common-->libitsameme_util;

libitsameme_kernel-->libitsameme_consensus;
libitsameme_kernel-->libitsameme_util;

libitsameme_node-->libitsameme_consensus;
libitsameme_node-->libitsameme_kernel;
libitsameme_node-->libitsameme_common;
libitsameme_node-->libitsameme_util;

libitsamemeqt-->libitsameme_common;
libitsamemeqt-->libitsameme_util;

libitsameme_wallet-->libitsameme_common;
libitsameme_wallet-->libitsameme_util;

libitsameme_wallet_tool-->libitsameme_wallet;
libitsameme_wallet_tool-->libitsameme_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class itsameme-qt,itsamemed,itsameme-cli,itsameme-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libitsameme_wallet* and *libitsameme_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libitsameme_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libitsameme_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libitsameme_common* should serve a similar function as *libitsameme_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libitsameme_util* and *libitsameme_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for itsameme-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libitsameme_kernel* is not supposed to depend on *libitsameme_common*, only *libitsameme_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libitsameme_kernel* should only depend on *libitsameme_util* and *libitsameme_consensus*.

- The only thing that should depend on *libitsameme_kernel* internally should be *libitsameme_node*. GUI and wallet libraries *libitsamemeqt* and *libitsameme_wallet* in particular should not depend on *libitsameme_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libitsameme_consensus*, *libitsameme_common*, and *libitsameme_util*, instead of *libitsameme_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libitsamemeqt*, *libitsameme_node*, *libitsameme_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libitsameme_node* to *libitsameme_kernel* as part of [The libitsamemekernel Project #24303](https://github.com/itsamemedev/ITSAMEME/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/itsamemedev/ITSAMEME/issues/15732)
