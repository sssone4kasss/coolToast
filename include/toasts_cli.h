#include "toasts.h"


class CoolToastCLI {
public:
  
    CoolToastCLI(const CoolToast& ct);

    void start();

private:

    const CoolToast cool_toast; ///< Обьект для работы с тостами 
    bool should_exit;           ///< Флаг для контроля завершения программы  
};
